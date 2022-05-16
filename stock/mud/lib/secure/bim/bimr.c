// BIM: Bubbs' Intermud Mailer.
// bimr.c  :  Mail reader / sender.
// This object is used when a user enters mail.
// It is the main body of the mail command.  It
// translates commands, builds mail, and also
// reads it and various other tricks and toys.


#include <bim.h>

#define PAGER "obj/pager"

#define localtime(Q,T) (T)

#if defined(SML_SET_OF) || defined(SML_SUBST) || defined(SML_SUM_ARRAY) || defined(SML_TRIMSTR)
inherit BIM_DIR"simuls";
#endif


// Variable declarations.
mapping *messages,                    // This user's messages.
         mgroups;                     // This user's own groups.

// Static variables.
static   int      current_mesg;       // The current position.
static   status   in_menu;            // Are we using the menu ?


// Macro definitions.
#define STATUS_LINE(i) \
  sprintf("%c%3d%c%c",\
   (i) == current_mesg               ? '*' : ' ',\
   (i)+1,\
   messages[i][FLAGS]    & IF_MARKED ? 'M' :\
    (messages[i][FLAGS]  & IF_STORED ? 'S' : ' '),\
   messages[i][FLAGS]    & IF_NEW    ? 'N' :\
    (messages[i][FLAGS]  & IF_UNREAD ? 'U' :\
     (messages[i][FLAGS] & IF_KEPT   ? 'K' : ' ')))

#define CAP_LINE(l, d) \
  implode(map_array(explode(l, d), #'capitalize), d)


// Signiture begin and end markers.
// Note that they are both 'invisible' when printed.
#define BEGIN_SIG    " \b \b"
#define END_SIG      "  \b\b"


// Function prototypes.
void    menu();
void    parse_cmd(string str);
void    ask_reply_include(string str, int index);
void    build_mail(mapping mesg);
void    ask_mail_field(string label, mapping mesg,
                       string field, string  default_field);
void    delete_marked_messages();
void    send_menu(mapping mesg);
void    parse_send_cmd(string str, mapping mesg);
void    quit_mail(string cmd, int num_mesgs);
void    quit_mail2(string str, string cmd, int num_mesgs);
void    quit_mail3(string str, int *range, int excess);

status  strip_sig(string *text);
string  valid_destination(string dest);
status  check_dir(string file);
string  get_message(mapping mesg, string header);
int     get_single(string str);
int    *get_range(string rng);
int    *get_sub_range(string bit);

//// function by Geewhiz /////////////
varargs string date(int t);
///////////////////////////////////// 


// Initiate the data for this user.
void init_bim(status inmenu) {
  int index, size;
  restore_object(MAIL_SAVE(MNAME(this_player())));
  if (inmenu)
   (void)BIM_DIR"bimd"->set_mailer(this_player(), this_object());
  if (!messages) {
    messages = ({ });
    mgroups = ([ ]);
    }
  in_menu = inmenu;
  current_mesg = -1;
  size = sizeof(messages);
  for(index = 0; index < size && current_mesg == -1; index++)
   if (messages[index][FLAGS] & IF_UNREAD ||
       messages[index][FLAGS] & IF_NEW)
    current_mesg = index;
  if (current_mesg == -1)
   current_mesg = size-1;
  return;
  }


// The main menu.  (well... prompt really)
void menu() {
  if (!in_menu) {
    (void)BIM_DIR"bimd"->set_mailer(this_player(), 0);
    return;
    }
  if (sizeof(messages))
   printf("%s"BIM_PROMPT, STATUS_LINE(current_mesg));
  else
   write(BIM_PROMPT);
  input_to("parse_cmd");
  return;
  }


// The command parser.
void parse_cmd(string str) {
  mixed tmp;
  mapping mesg;
  string cmd, allargs, *args, *keys;
  int *range, num_mesgs, index, size;
  args = explode(str, " ");
  cmd = args[0];
  args = args[1..];
  allargs = implode(args, " ");
  num_mesgs = sizeof(messages);
  switch(cmd) {
    case "f":
    case "for":   case "forward":
      if (num_mesgs == 0)
       return menu();
      if (allargs == "") allargs = "*";
      index = get_single(allargs);
      if (index == -1)
       index = current_mesg;
      else
       allargs = 0;
      mesg = ([
       FWD_BY       : capitalize(MNAME(this_player())),
       ORIG RCPT    : messages[index][RCPT],
       ORIG SNDR    : messages[index][SNDR],
       DEFAULT SUBJ : messages[index][SUBJ],
       ORIG SUBJ    : messages[index][SUBJ],
       MESSAGE      : messages[index][MESSAGE],
       ]);
      if (allargs) mesg[RCPT] = allargs;
      build_mail(mesg);
      return;
    break;
    case "g":
    case "go":
      index = get_single(allargs);
      if (index != -1)
       current_mesg = index;
    break;
    case "group":  case "gr":  case "G":
      if (allargs == "") {
        size = sizeof(keys = m_indices(mgroups));
        if (!size) write("No mail groups defined.\n");
        else {
          printf("%d mail group%c defined:\n",
           size, size == 1 ? 0 : 's');
          keys = sort_array(keys, #'>);
          for(index = 0; index < size; index++)
           printf("%-15s  %-60=s\n",
            keys[index], implode(mgroups[keys[index]], ", "));
          }
        }
      else {
        string arg1, arg2;
        if (sscanf(allargs, "%s %s", arg1, arg2) != 2) {
          if (member(mgroups, allargs) == 0)
           printf("No such mail group: \"%s\".\n", allargs);
          else
           printf("%-15s  %-60=s\n",
            allargs, implode(mgroups[allargs], ", "));
          }
        else {
          arg1 = trimstr(arg1);
          if (arg2[0] == '+' || arg2[0] == '-' || arg2[0] == '=')
           tmp = arg2[0], arg2 = trimstr(arg2[1..]);
          else tmp = '=';
          keys = explode(trimstr(arg2), ",") - ({ "" });
          keys = map_array(keys, #'trimstr);
          if (member(mgroups, arg1) == 0)
           mgroups[arg1] = ({ });
          switch(tmp) {
            case '=':
              mgroups[arg1] = keys;
            break;
            case '+':
              mgroups[arg1] += keys;
            break;
            case '-':
              mgroups[arg1] -= keys;
            break;
            }
          mgroups[arg1] = set_of(mgroups[arg1]);
          if (sizeof(mgroups[arg1]) == 0) {
            m_delete(mgroups, arg1);
            printf("Mail group \"%s\" deleted.\n", arg1);
            }
          else
           printf("%-15s  %-60=s\n",
            arg1, implode(mgroups[arg1], ", "));
          }
        }
    break;
    case "h":
    case "help":  case "?":
      switch(allargs) {
        case "":
          printf("           BIM v"BIM_VERSION".\n"
                 "f [<mesg>|<name>]   Forward message.\n"
                 "g <mesg>            Change current message to <mesg>.\n"
                 "group [<gr> <def>]  View or set personal mail groups.\n"
                 "h [<topic>]         Get help on <topic>.\n"
                 "l [<range>]         List mail headers in <range>.\n"
                 "m [<range>]         Mark messages in <range>.\n"
                 "n                   Read next unread message.\n"
                 "p [<mesg>]          Read [current] message.\n"
                 "%s"
                 "q                   Save, and quit.\n"
                 "r [<mesg>]          Reply to [current] message.\n"
                 "S [<range>]         Toggle 'Stored' flag on message.\n"
                 "s [<dest>]          Send mail to <dest>.\n"
                 "t                   Tidy (delete) marked messages.\n"
                 "u [<range>]         Unmark messages in <range>.\n",
           QUIT_NO_SAVE(this_player()) ?
                 "Q                   Quit without saving.\n" : "");
          if (ALLOW_R_W_FILE(this_player()))
           write("w <file>            Write current message to <file>.\n");
          write(
#if defined(X_QUICK_QUIT)
                "x                   Tidy, save and quit.\n"
#endif   // defined(X_QUICK_QUIT)
                "<num>               Read message <num>.\n"
                "+<num>              Go forward <num> messages.\n"
                "-<num>              Go backward <num> messages.\n"
                "\n"
                "Other topics: bim, groups, quota, range, version.\n");
        break;
        case "bim": case "BIM":
          write("BIM: Bubbs' Intermud Mailer (v"BIM_VERSION")\n"
                "BIM is no longer just an 'intermud' mailer.  It\n"
                "can be pushed in many directions, one of which is\n"
                "Nostradamus@Zebedee's intermud network.  BIM can\n"
                "be used on any LPMud that runs the 3.2[.1] driver.\n"
                "\n"
                "Come and see BIM's home, at Timewarp, we are at\n"
                "      quark.gmi.edu (192.138.137.39) 5150\n"
                "\n"
                "You'll find me there.  And a few others probably.\n"
                "\n"
                "Bubbs@Timewarp              (bubbs@quark.gmi.edu)\n");
        break;
        case "f": case "for": case "forward":
          write("Usage: f [<mesg>|<name>]    (also: for, forward)\n"
                "Forward a message on.\n");
        break;
        case "g": case "go":
          write("Usage: g <mesg>      (also: go, goto)\n"
                "Change current message to <mesg>.\n");
        break;
        case "groups": {
          mapping G;
          G = (mapping)BIM_DIR"bimd"->query_Groups();
          size = sizeof(keys = sort_array(m_indices(G), #'>));
          printf("Groups\n"
                 "Mail groups are 'names' for a multiple group of users.\n"
                 "When you mail a group, in effect, you mail each member\n"
                 "of that group.  Some groups are defined by the mailer,\n"
                 "but you can also define some for yourself, using the\n"
                 "'G' command.  There are %s group%c defined by the mailer%c\n",
           size ? to_string(size) : "no",
           size == 1 ? 0 : 's',
           size ? ':' : '.');
          for(index = 0; index < size; index++)
           printf("%-15s  %-60=s\n",
            keys[index], implode(G[keys[index]], ", "));
          }
        break;
        case "group": case "G": case "gr":
          write("Usage: group [<gr> [[=|+|-] <def>]]      (also: G, gr)\n"
                "View, set, or modify personal mail groups.\n");
        break;
        case "h": case "help": case "?":
          write("Usage: h [<topic>]   (also: help, ?)\n"
                "Get help on a <topic>.\n");
        break;
        case "l": case "list":
          write("Usage: l [<range>]   (also: list)\n"
                "List message headers in <range>.\n");
        break;
        case "quota": case "quotas":
#if defined(STORE_QUOTA)
          index = STORE_QUOTA(this_player());
#else
          index = -1;
#endif   // defined(STORE_QUOTA)
#if defined(SIZE_QUOTA)
          size  = SIZE_QUOTA(this_player());
#else
          size  = -1;
#endif   // defined(SIZE_QUOTA)
          printf("There are two quotas that may be defined.  They\n"
                 "are 'store' and 'size'.  The 'store' quota is\n"
                 "the maximum number of messages that may be stored\n"
                 "(see the 'S' command).  The 'size' quota is the\n"
                 "maximum number of read or non-stored messages\n"
                 "that you may have when you quit mail.  If you\n"
                 "have more, they will be automatically deleted.\n"
                 "\n"
                 "Your 'store' quota is: %s.\n"
                 "Your 'size'  quota is: %s.\n"
                 "\n",
           index < 0 ? "no limit" :
                       (index ? to_string(index) : "none"),
           size < 0  ? "no limit" :
                       (size  ? to_string(size)  : "none"));
        break;
        case "m": case "mark": case "d":
          write("Usage: m [<range>]   (also: mark, d)\n"
                "Mark messages in <range>.\n");
        break;
        case "N":
          write("Usage: N [<range>]\n"
                "Mark messages in <range> as NEW and UNREAD.\n");
        break;
        case "n": case "next":
          write("Usage: n             (also: next)\n"
                "Read next unread message.\n");
        break;
        case "O":
          write("Usage: O [<range>]\n"
                "Remove NEW and UNREAD flags from messages in <range>.\n");
        break;
        case "p": case "read": case "pr": case "print":
          write("Usage: p [<mesg>]    (also: read, print)\n"
                "Print message <mesg>.\n");
        break;
        case "Q":
          write(QUIT_NO_SAVE(this_player()) ?
           "Usage: Q\n"
           "Quit without saving.\n" :
           "Unknown topic.\n");
        break;
#if !defined(X_QUICK_QUIT)
        case "q": case "quit": case "x":
          write("Usage: q             (also: quit, x)\n"
                "Save and quit.\n");
        break;
#else
        case "q": case "quit":
          write("Usage: q             (also: quit)\n"
                "Save and quit.\n");
        break;
        case "x":
          write("Usage: x\n"
                "Tidy, save, and quit.\n");
        break;
#endif   // !defined(X_QUICK_QUIT)
        case "S": case "store":
          write("Usage: S [<range>]   (also: store)\n"
                "Toggle the 'Stored' flag on a range of messages.\n"
                "Stored messages do not count toward the number of\n"
                "messages you may have in your mailbox.\n");
        break;
        case "range": case "ranges":
          write("Ranges\n"
                "A range is a list of messages.\n"
                "It is keyword, or a string in the form: \"<num>..<num>\".\n"
                "Valid keywords are:\n"
                "        all        all messages.\n"
                "        k          all kept messages.\n"
                "        m          all marked messages.\n"
                "        n          all new messages.\n"
                "        s          all stored messages.\n"
                "        u          all unread messages.\n"
                "        uns        all unstored messages.\n"
                "In the \"<num>..<num>\" form, the <num>'s can be\n"
                "relative to the current message, eg: \"-4..+4\".\n");
        break;
        case "r": case "reply":
          write("Usage: r [<mesg>]    (also: reply)\n"
                "Reply to message <mesg>.\n");
        break;
        case "s":
        case "send":   case "mail":
          write("Usage: s [<recipient>]     (also: send, mail)\n"
                "Send mail to <recipient>.\n");
        break;
        case "t": case "tidy":
          write("Usage: t              (also: tidy)\n"
                "Delete marked messages.\n");
        break;
        case "u": case "unmark":
          write("Usage: u [<range>]    (also: unmark)\n"
                "Unmark a range of messages.\n");
        break;
        case "version":
          write("BIM: Bubbs' Intermud Mailer.\n"
                "v1.0   24/Aug/94.    Original coding.\n"
                "v1.1   12/Sep/94.    Fixed a few bugs.\n"
                "v1.2   24/Sep/94.    Fixed a few more bugs, added AUTO_* support.\n"
                "v1.3    6/Oct/94.    Incorporated editor and pager for portability.\n"
                "v1.31  24/Oct/94.    Fixed a nasty bug in match_mud stuff.\n"
                "v1.32   5/Nov/94.    Fixed some more bugs.\n"
                "v1.4   17/Dec/94.    Bug fixes, tidy ups.\n"
                "v1.41  14/Jan/94.    Bug fixes, mailer object is now stored internally.\n"
                "v1.42  29/Jan/94.    More options, more upgrades.\n"
                "v2.0   13/May/95.    Mail Daemon support.  Intermud mail handled by daemon.\n"
                "v2.01  28/May/95.    A couple of fixes to mail daemon handling.\n"
                "v2.02  30/May/95.    Some more fixes to mail daemon stuff.\n"
                "v2.03   4/Jun/95.    Tidy ups, and more information.\n"
                "v2.04  10/Jun/95.    Fixed a bug in Mail Daemon stuff.\n"
                "v2.05  20/Jun/95.    Just a couple of small improvements.\n"
                "v2.06  23/Jun/95.    Added optional mail quotas.\n"
                "v2.1   23/Jun/95.    Upped patch level to 2.1.\n"
                "v2.11  11/Jul/95.    A couple of patches to quota stuff.\n"
                "\n");
        break;
        case "w": case "W": case "write":
          if (ALLOW_R_W_FILE(this_player()))
           write("Usage: w <file>       (also: W, write)\n"
                 "Write a message to file.\n"
                 "\"W\" will write the body of the message only.\n");
          else
           write("Unknown topic.\n");
        break;
        case "+":
          write("Usage: +<num>\n"
                "Move current message forwards <num> messages.\n");
        break;
        case "-":
          write("Usage: -<num>\n"
                "Move current message backwards <num> messages.\n");
        break;
        default:
          write("Unknown topic.\n");
        break;
        }
    break;
    case "l":
    case "list":
      if (allargs == "") allargs = "all";
      size = sizeof(range = get_range(allargs));
//    if (!size)
//     write("No messages.\n");
      for(index = 0; index < size; index++)
       printf("%6s: %10s  %:-15s %:-43=s\n",
        STATUS_LINE(range[index]),
        date(messages[range[index]][DATE]),
        messages[range[index]][S_MUD] ?
         sprintf("%s@%s",
          capitalize(messages[range[index]][SNDR]),
          messages[range[index]][S_MUD]) :
         capitalize(messages[range[index]][SNDR]),
        messages[range[index]][SUBJ]);
    break;
    case "m":
    case "mark":  case "d":
      if (allargs == "") allargs = "*";
      size = sizeof(range = get_range(allargs));
      for(index = 0; index < size; index++) {
        if (messages[range[index]][FLAGS] & IF_MARKED) {
          printf("Message %d already marked.\n",
           range[index]+1);
          range = range[0..index-1] + range[index+1..];
          index--;
          size--;
          }
        else
         messages[range[index]][FLAGS] |= IF_MARKED;
        if (sizeof(range) && range[index] == current_mesg &&
         current_mesg != num_mesgs - 1)
          current_mesg++;
        }
      printf("%s message%c marked.\n",
       size ? to_string(size) : "No",
       size == 1 ? 0 : 's');
    break;
    case "N":
      if (allargs == "") allargs = "*";
      size = sizeof(range = get_range(allargs));
      for(index = 0; index < size; index++) {
        if (messages[range[index]][FLAGS] & IF_NEW) {
          printf("Message %d already new.\n",
           range[index]+1);
          range = range[0..index-1] + range[index+1..];
          index--;
          size--;
          }
        else
         messages[range[index]][FLAGS] |= IF_NEW | IF_UNREAD;
        }
      printf("%s message%c renewed.\n",
       size ? to_string(size) : "No",
       size == 1 ? 0 : 's');
      if (size)
       current_mesg = range[<1];
    break;
    case "n":
    case "next":  case "":
      parse_cmd("print next unread");
      return;
    break;
    case "p":     case "read":
    case "pr":    case "print":
      if (allargs == "") allargs = "*";
      index = get_single(allargs);
      if (index != -1) {
        object pager;
        messages[index][FLAGS] -= IF_NEW    & messages[index][FLAGS];
        messages[index][FLAGS] -= IF_UNREAD & messages[index][FLAGS];
        if (AUTO_MARK(this_player()))
         messages[index][FLAGS] |= IF_MARKED;
        current_mesg = index;
#undef BIM_PAGER
#define TIMEWARP
#if defined(BIM_PAGER)
        (void)BIM_DIR"pager"->start_pager(([
         PG_BUFF  : explode(get_message(messages[index],
                     to_string(index+1)), "\n"),
         PG_Q_FUNC: "quit_pager",
         ]));
#elif defined(TIMEWARP)
        pager = clone_object(PAGER);
        (void)pager->set_quit_func(this_object(), "TW_quit_pager");
        size = to_int((mixed)this_player()->query_env_var("pg_opts"));
        if (size)
         (void)pager->set_options(size);
        if(this_player()->defined_env_var("ml_length")) {
            size = to_int((mixed)this_player()->query_env_var("ml_length"));
            (void)pager->set_page_length(size);
        } else if(this_player()->defined_env_var("pg_length")) {
            size = to_int((mixed)this_player()->query_env_var("pg_length"));
            (void)pager->set_page_length(size);
        }
        (void)pager->more_text(explode(get_message(messages[index],
         to_string(index+1)), "\n"),
         sprintf("%d: %s   %s%s  %s",
          index+1,
         date(messages[index][DATE]),
          capitalize(messages[index][SNDR]),
          (messages[index][S_MUD] ? sprintf("@%s", messages[index][S_MUD]) : ""),
          messages[index][SUBJ]));
#else
        write("Error: Could not find a pager.\n");
        LOG("Could not find a pager.  (BIM_PAGER not defined).\n");
#endif   // defined(BIM_PAGER)
        return;
        }
    break;
    case "O":
      if (allargs == "") allargs = "*";
      size = sizeof(range = get_range(allargs));
      for(index = 0; index < size; index++) {
        if (((messages[range[index]][FLAGS] & IF_NEW   ) == 0) &&
            ((messages[range[index]][FLAGS] & IF_UNREAD) == 0)) {
          printf("Message %d already marked as read.\n",
           range[index]+1);
          range = range[0..index-1] + range[index+1..];
          index--;
          size--;
          }
        else
         messages[range[index]][FLAGS] -=
          messages[range[index]][FLAGS] & (IF_NEW | IF_UNREAD);
        }
      printf("%s message%c marked as read.\n",
       size ? to_string(size) : "No",
       size == 1 ? 0 : 's');
      if (size)
       current_mesg = range[<1];
    break;
    case "Q":
      if (QUIT_NO_SAVE(this_player())) {
        write("Quit without save.\n");
        (void)BIM_DIR"bimd"->set_mailer(this_player(), 0);
        return;
        }
      else
       write("Unrecognised command.\n");
    break;
    case "q":
    case "quit":  case "x":
      quit_mail(cmd, num_mesgs);
      return;
    break;
    case "reply":
    case "r":
      if (allargs == "") allargs = "*";
      index = get_single(allargs);
      if (index != -1) {
        switch(REPLY_INCLUDE(this_player())) {
          case "yes":
            ask_reply_include("yes", index);
          break;
          case "no":
            ask_reply_include("no", index);
          break;
          default:
            write("Include previous message ? (y/N/q) ");
            input_to("ask_reply_include", 0, index);
          break;
          }
        return;
        }
    break;
    case "s":
    case "send":   case "mail":
      build_mail(([ RCPT: allargs ]));
      return;
    break;
    case "st":    case "S":
    case "store": {
      int quota;
      if (allargs == "") allargs = "*";
      tmp = ({ });
      for(index = 0; index < num_mesgs; index++)
       if (messages[index][FLAGS] & IF_STORED)
        tmp += ({ index });
#if defined(STORE_QUOTA)
      quota = STORE_QUOTA(this_player());
#else
      quota = -1;
#endif   // defined(STORE_QUOTA)
      size = sizeof(range = get_range(allargs));
      for(index = 0; index < size; index++) {
        if (messages[range[index]][FLAGS] & IF_STORED) {
          messages[range[index]][FLAGS] -= IF_STORED;
          tmp -= range[index..index];
          }
        else if (quota != -1 && sizeof(tmp) >= quota)
         write("Cannot store any more messages.\n");
        else {
          messages[range[index]][FLAGS] |= IF_STORED;
          tmp += range[index..index];
          }
        printf("Message %d %s stored.\n",
         range[index]+1,
         messages[range[index]][FLAGS] & IF_STORED ?
          "is now" : "no longer");
        }
      if (size)
       current_mesg = range[<1];
      }
    break;
    case "t":    case "T":
    case "tidy":
      delete_marked_messages();
    break;
    case "u":
    case "unmark":
      if (allargs == "") allargs = "*";
      size = sizeof(range = get_range(allargs));
      for(index = 0; index < size; index++) {
        if (messages[range[index]][FLAGS] & IF_MARKED)
         messages[range[index]][FLAGS] -= IF_MARKED;
        else {
          printf("Message %d not marked.\n",
           range[index]+1);
          range = range[0..index-1] + range[index+1..];
          index--;
          size--;
          }
        }
      printf("%s message%c unmarked.\n",
       size ? to_string(size) : "No",
       size == 1 ? 0 : 's');
      if (size)
       current_mesg = range[<1];
    break;
    case "w": case "W":
    case "write":
      if (ALLOW_R_W_FILE(this_player())) {
        string file, file2, text;
        if (sizeof(messages) == 0)
         return menu();
        text = get_message(messages[current_mesg],
         cmd == "W" ? 0 : to_string(current_mesg+1));
        if (text[<1] != '\n')
         text = sprintf("%s\n", text);
        if ((file = trimstr(allargs)) == "")
         file = DEFAULT_MAIL_FILE;
#if defined(EVAL_PATH)
        file = EVAL_PATH(file);
#endif   // defined(EVAL_PATH)
        file = (mixed)__MASTER_OBJECT__->valid_write(file2 = file,
         MNAME(this_player()), "write_file", this_player());
        if (!file)
         printf("No write access to %O.\n", file2);
        else if (!check_dir(file))
         printf("Cannot open %O to write.\n", file);
        else if (file_size(file) == -2)
         printf("Cannot write to directory: %O.\n", file);
        else
         printf(write_file(file, text) ?
          "Mail written to: %s.\n" :
          "Failed to write to: %s.\n",
          file);
        }
      else
       write("Unrecognised command.\n");
    break;
    case "+":
    case "-":
      return
       parse_cmd(sprintf("go %d",
        current_mesg + (cmd == "+" ? 2 : 0)));
    break;
    default:
      index = get_single(str);
      if (index != -1)
       return
        parse_cmd(sprintf("read %d", index+1));
      else
       write("Unrecognised command.\n");
    break;
    }
  menu();
  return;
  }


// Handle the reply command.  We must do this
// seperately to enable the "ask" include option.
void ask_reply_include(string str, int index) {
  string *txt, whoto, subj;
  int ind;
  str = lower_case(str);
  if (str == "q") {
    write("Reply aborted.\n");
    menu();
    return;
    }
  txt = explode(messages[index][SNDR], "@");
  if (sizeof(txt) > 2)
   txt = txt[0..1];
  whoto = implode(txt, "@");
  subj = messages[index][SUBJ];
  if (sscanf(subj, "Re^%d: %s", ind, subj) != 2 ||
      sscanf(subj, "RE^%d: %s", ind, subj) != 2) {
    if (subj[0..3] == "Re: " || subj[0..3] == "RE: ")
     ind = 1, subj = subj[4..];
    else
     ind = 0, subj = sprintf("Re: %s", subj);
    }
  if (str == "y" || str == "ye" || str == "yes") {
    int size;
    txt = explode(messages[index][MESSAGE], "\n");
    strip_sig(&txt);
    size = sizeof(txt);
    for(ind = 0; ind < size; ind++)
     txt[ind] = sprintf("> %s", txt[ind]);
    txt[0..0] = ({
     sprintf("%s writes:",
      capitalize(messages[index][SNDR])),
     txt[0]
     });
    }
  else
   txt = 0;
  build_mail(([
   DEFAULT RCPT   : whoto,
   ORIG RCPT      : messages[index][RCPT],
   ORIG SNDR      : messages[index][SNDR],
   DEFAULT SUBJ   : subj,
   ORIG SUBJ      : messages[index][SUBJ],
   DEFAULT MESSAGE: txt,
   DEFAULT CCOPY  : messages[index][CCOPY],
   ]));
  }


// Write some new mail.
void build_mail(mapping mesg) {
  string *tmp;
  int size;
  if (!mesg[RCPT]) {
    if (member(mesg, RCPT))
     write("You must give at least one recipient.\n");
    return
     ask_mail_field("To", mesg, RCPT, DEFAULT RCPT);
    }
  size = sizeof(tmp = map_array(explode(mesg[RCPT], ","), #'trimstr));
  tmp = map_array(tmp, #'valid_destination) - ({ 0 });
  if (size != sizeof(tmp))
   return
    menu();
  else
   mesg[RCPT] = implode(set_of(tmp), ", ");
  if (!member(mesg, SNDR))
   mesg[SNDR] = MNAME(this_player());
  if (!member(mesg, SUBJ))
   return
    ask_mail_field("Subject", mesg, SUBJ, DEFAULT SUBJ);
  if (!mesg[SUBJ]) {
    write("You must give a subject.\n");
    return
     menu();
    }
  if (strlen(mesg[SUBJ]) > 25) {
    write("THE SUBJECT MUST BE SHORTER-- CONCENTRATE.\n");
    return
     menu();
    }
  if (!member(mesg, MESSAGE)) {
    object editor;
    string *text, sigbuff;
    int curr;
    status force_input;
    text = mesg[DEFAULT MESSAGE];
    if (!text) text = ({ });
    if (sizeof(text))
     strip_sig(&text);
    force_input = ((curr = sizeof(text)) == 0);
    if (sigbuff = READ_SIG(MNAME(this_player()))) {
      string *sig;
      tmp = explode(sigbuff, "\n")[0..<2];
      tmp[0]  = sprintf(BEGIN_SIG"%s", tmp[0]);
      tmp[<1] = sprintf(END_SIG"%s",   tmp[<1]);
      text += tmp;
      }
#if defined(BIM_EDITOR)
    (void)BIM_DIR"editor"->start_editing(([
     ED_MAILR   : this_object(),
     ED_BUFF    : text,
     ED_Q_FUNC  : "quit_write",
     ED_CURR    : curr,
     ED_EXTRA   : mesg
     ]));
#elif defined(TIMEWARP)
    editor = clone_object(EDITOR);
    (void)editor->set_quit_func(this_object(), "TW_quit_write");
    (void)editor->set_text_func("get_message_text");
    (void)editor->set_deadbuff("data/mail/deadmail/");
    (void)editor->set_extra(mesg);
    (void)editor->edit_text(text, force_input);
#else
    write("Error: Could not find an editor.\n");
    LOG("Could not find an editor.  (BIM_EDITOR not defined).\n");
#endif   // defined(BIM_EDITOR)
    return;
    }
  if (!member(mesg, CCOPY))
   return
    ask_mail_field("Copy to", mesg, CCOPY, DEFAULT CCOPY);
  else if (mesg[CCOPY] == " ") {
    write("Copy to field cleared.\n");
    mesg[CCOPY] = 0;
    }
  else if (mesg[CCOPY]) {
    tmp = map_array(explode(mesg[CCOPY], ","), #'trimstr);
    tmp = set_of(map_array(tmp, #'valid_destination) - ({ 0 }));
    mesg[CCOPY] = (sizeof(tmp) ? implode(set_of(tmp), ", ") : 0);
    }
  if (!member(mesg, DATE))
   mesg[DATE] = time();
  send_menu(mesg);
  return;
  }


// Ask for a new mail field.
void ask_mail_field(string label, mapping mesg,
                    string field, string  default_field) {
  printf("%s%s: ",
   label,
   mesg[default_field] ?
    sprintf(" [%s]", mesg[default_field]) : "");
  input_to("get_mail_field", 0, mesg, field, default_field);
  return;
  }


// Get mail data for build_mail().
void get_mail_field(string str, mapping mesg,
                    string field, string default_field) {
  mesg[field] = (str == "" ? mesg[default_field] : str);
  m_delete(mesg, default_field);
  build_mail(mesg);
  return;
  }


// Support for the edit object.
#if defined(BIM_EDITOR)
void quit_write(mapping data) {
  mapping mesg;
  string *text;
  text = data[ED_BUFF];
  mesg = data[ED_EXTRA];
#else
void TW_quit_write(string *text, object editor, mapping mesg) {
  destruct(editor);
#endif   // defined(BIM_EDITOR)
  if (!text) {
    write("Aborted.\n");
    menu();
    return;
    }
  mesg[MESSAGE] = implode(text, "\n");
  build_mail(mesg);
  return;
  }


// Used by the editor to get mail messages.
string *get_message_text(int index) {
  string *text;
  if (index < 1 || index > sizeof(messages)) {
    printf("Message %d does not exist.\n", index);
    return 0;
    }
  return explode(get_message(messages[index - 1], to_string(index)), "\n");
  }


// Format a message for output.
string get_message(mapping mesg, string header) {
  string txt;
  txt = "";
  if (mesg[CCOPY])
   txt = sprintf("Copied to: %s\n", CAP_LINE(mesg[CCOPY], ", "));
  if (mesg[FWD_BY])
   txt = sprintf("%sForwarded: %s\n", txt, mesg[FWD_BY]);
  if (mesg[MAIL_GROUP])
   txt = sprintf("%sGroup mailed: %s\n", txt, mesg[MAIL_GROUP]);
  if (mesg[ORIG RCPT])
   txt = sprintf("%sOriginal recipient: %s\n",
    txt, CAP_LINE(mesg[ORIG RCPT], ", "));
  if (mesg[ORIG SNDR])
   txt = sprintf("%sOriginal sender   : %s\n",
    txt, capitalize(mesg[ORIG SNDR]));
  if (mesg[ORIG SUBJ])
   txt = sprintf("%sOriginal subject  : %s\n", txt, mesg[ORIG SUBJ]);
  if (mesg[ORIG CCOPY])
   txt = sprintf("%sOriginal copies to: %s\n",
    txt, CAP_LINE(mesg[ORIG CCOPY], ", "));
  txt = sprintf("Date sent: %s\n"
                "%s\n"
                "%s",
#if defined(TIMEWARP) || defined(DUNE)
   ctime(localtime(0, mesg[DATE]))[0..18],
#else
   ctime(mesg[DATE])[0..18],
#endif   // defined(TIMEWARP) || defined(DUNE)
   txt,
   mesg[MESSAGE]);
  if (header)
   txt = sprintf("Message  : %s\n"
                 "Recipient: %s\n"
                 "From     : %s%s\n"
                 "Subject  : %s\n"
                 "%s",
    header,
    CAP_LINE(mesg[RCPT], ", "),
    capitalize(mesg[SNDR]),
    mesg[S_MUD] ? sprintf("@%s", mesg[S_MUD]) : "",
    mesg[SUBJ],
    txt);
  else
   txt = mesg[MESSAGE];
  return txt;
  }


// Support for pager.
#if defined(BIM_PAGER)
void quit_pager(mapping data) {
#else
void TW_quit_pager(string reason, object pager, mixed extra) {
  if (!reason) write("Terminated.\n");
  destruct(pager);
#endif   // defined(BIM_PAGER)
  menu();
  return;
  }


// Detect a valid destination.
string valid_destination(string str) {
  return (member(mgroups, str) ||
   (status)BIM_DIR"bimd"->valid_destination(&str) ? str : 0);
  }


// The penultimate stage before sending the mail.
void send_menu(mapping mesg) {
  if (mesg[COMPLETE]) {
    m_delete(mesg, COMPLETE);
    parse_send_cmd("s", mesg);
    return;
    }
  write("(S)end, (E)dit, (A)bort or (?)Help.\n"
        "What now ? ");
  input_to("parse_send_cmd", 0, mesg);
  return;
  }


// The final stage before sending the mail.
void parse_send_cmd(string str, mapping mesg) {
  string text;
  str = lower_case(str);
  switch(str[0]) {
    case 's':
    case 'x': {
      mixed *rcpts, *ccopy;
      int index, size;
      rcpts = explode(mesg[RCPT], ", ");
      size = sizeof(rcpts);
      for(index = 0; index < size; index++)
       if (mgroups[rcpts[index]])
        rcpts[index..index] = mgroups[rcpts[index]];
      mesg[RCPT] = implode(rcpts, ", ");
      if (mesg[CCOPY]) {
        ccopy = explode(mesg[CCOPY], ", ");
        size = sizeof(ccopy);
        for(index = 0; index < size; index++)
         if (mgroups[ccopy[index]])
          ccopy[index..index] = mgroups[ccopy[index]];
        mesg[CCOPY] = implode(ccopy, ", ");
        rcpts += ccopy;
        }
      rcpts = map_array(rcpts, "eval_groups", BIM_DIR"bimd");
      rcpts = set_of(sum_array(rcpts));
      map_array(rcpts, "send_mail", BIM_DIR"bimd", mesg);
      menu();
      return;
      }
    break;
    case 'k': {
      mapping msg;
      msg = copy_mapping(mesg);
      msg[FLAGS] = IF_KEPT;
      messages += ({ msg });
      if (!in_menu)
       save_object(MAIL_SAVE(MNAME(this_player())));
      parse_send_cmd("s", mesg);
      return;
      }
    break;
    case 'e': {
#if defined(BIM_EDITOR)
      (void)BIM_DIR"editor"->start_editing(([
       ED_BUFF    : explode(mesg[MESSAGE], "\n"),
       ED_Q_FUNC  : "quit_edit",
       ED_EXTRA   : mesg,
       ]));
#elif defined(TIMEWARP)
      object editor;
      editor = clone_object(EDITOR);
      (void)editor->set_quit_func(this_object(), "TW_quit_edit");
      (void)editor->set_text_func("get_message_text");
      (void)editor->set_deadbuff("data/mail/deadmail/");
      (void)editor->set_extra(mesg);
      (void)editor->edit_text(explode(mesg[MESSAGE], "\n"), 0);
#else
      write("Error: Could not find an editor.\n");
      LOG("Could not find an editor.  (BIM_EDITOR not defined).\n");
#endif   // defined(BIM_EDITOR)
      return;
      }
    break;
    case '?':
      write("Send Menu:\n"
            "s            Send mail.\n"
            "k            Send mail, but keep a copy.\n"
            "a            Abort send.\n"
            "e            Edit mail body.\n"
            "h            Edit headers.\n"
            "v            View mail.\n"
            "?            This help page.\n");
      if (ALLOW_R_W_FILE(this_player()))
       write("w <file>     Write draft mail to <file>.\n");
    break;
    case 'h':
      mesg[DEFAULT RCPT]  = mesg[RCPT];
      mesg[DEFAULT SUBJ]  = mesg[SUBJ];
      mesg[DEFAULT CCOPY] = mesg[CCOPY];
      m_delete(mesg, RCPT);
      m_delete(mesg, SUBJ);
      m_delete(mesg, CCOPY);
      build_mail(mesg);
      return;
    break;
    case 'v': {
#if defined(BIM_PAGER)
      (void)BIM_DIR"pager"->start_pager(([
       PG_BUFF  : explode(get_message(mesg, "Draft"), "\n"),
       PG_Q_FUNC: "quit_view",
       PG_EXTRA : mesg,
       ]));
#elif defined(TIMEWARP)
      object pager;
      int opts;
      pager = clone_object(PAGER);
      (void)pager->set_quit_func(this_object(), "TW_quit_view");
      opts = to_int((string)this_player()->query_env_var("page_opts"));
      if (opts)
       (void)pager->set_options(opts);
      opts = to_int((string)this_player()->query_env_var("page_leng"));
      if (opts)
       (void)pager->set_page_length(opts);
      (void)pager->set_extra(mesg);
      text = get_message(mesg, "Draft");
      (void)pager->more_text(explode(text, "\n"),
       sprintf("%s: %s%s \"%s\"",
        "Draft",
        capitalize(mesg[SNDR]),
        (mesg[S_MUD] ? sprintf("@%s", mesg[S_MUD]) : ""),
        mesg[SUBJ]));
#else
      write("Error: Could not find a pager.\n");
      LOG("Could not find a pager.  (BIM_PAGER not defined).\n");
#endif   // defined(BIM_PAGER)
      return;
      }
    break;
    case 'w':
      if (ALLOW_R_W_FILE(this_player())) {
        string file, file2;
        text = get_message(mesg, "Draft");
        if (text[<1] != '\n')
         text = sprintf("%s\n", text);
        if ((file = trimstr(str[1..])) == "")
         file = DEFAULT_MAIL_FILE;
#if defined(EVAL_PATH)
        file = EVAL_PATH(file);
#endif   // defined(EVAL_PATH)
        file = (mixed)__MASTER_OBJECT__->valid_write(file2 = file,
         MNAME(this_player()), "write_file", this_player());
        if (!file)
         printf("No write access to %O.\n", file2);
        else if (!check_dir(file))
         printf("Cannot open %O to write.\n", file);
        else if (file_size(file) == -2)
         printf("Cannot write to directory %O.\n", file);
        else
         printf(write_file(file, text) ?
          "Draft mail written to: %s.\n" :
          "Failed to write to: %s.\n",
          file);
        }
     else
      write("Eh?\n");
    break;
    case 'a':
    case 'q':
      write("Aborting.\n");
      menu();
      return;
    break;
    default:
      write("Eh?\n");
    break;
    }
  send_menu(mesg);
  return;
  }


// Quit viewing the newly built message.
#if defined(BIM_PAGER)
void quit_view(mapping data) {
  mapping mesg;
  mesg = data[PG_EXTRA];
#else
void TW_quit_view(string reason, object pager, mapping mesg) {
  if (!reason) write("Terminated.\n");
  destruct(pager);
#endif   // defined(BIM_PAGER)
  send_menu(mesg);
  return;
  }


// Strip the signiture out of a message.
// Look for "invisible" markers of old signiture, so we can
// delete it.  A signiture starts with " \b \b", and ends
// with "  \b\b".
status strip_sig(string *text) {
  int index, size, beginsig, endsig;
  size = sizeof(text);
  for(index = 0, beginsig = -1; index < size && beginsig == -1; index++)
   if (text[index][0..3] == BEGIN_SIG) beginsig = index;
  for(endsig = -1; index < size && endsig == -1; index++)
   if (text[index][0..3] == END_SIG)   endsig = index;
  if (beginsig != -1 && endsig != -1)
   text = text[0..beginsig-1] + text[endsig+1..];
  return (sizeof(text) != size);
  }


// Get a single message number from a string.
int get_single(string str) {
  int *range, single;
  str = trimstr(str);
  if (str == "*") single = current_mesg;
  else if (str == "$") single = sizeof(messages) - 1;
  else if (str[0..5] == "first ") {
    range = get_range(str[6..]);
    single = (sizeof(range) ? range[0] : -1);
    }
  else if (str[0..4] == "next ") {
    range = filter_array(get_range(str[5..]), #'>=, current_mesg);
    single = (sizeof(range) ? range[0] : -1);
    }
  else if (str[0..4] == "last ") {
    range = get_range(str[5..]);
    single = (sizeof(range) ? range[<1] : -1);
    }
  else if (str[0] == '+' || str[0] == '-' ||
   (str[0] >= '0' && str[0] <= '9')) {
    single = to_int(str);
    if (str[0] == '+' || str[0] == '-')
     single += current_mesg;
    else single--;
    }
  else return -1;
  if (single < 0 || single >= sizeof(messages)) {
    write("No such message.\n");
    single = -1;
    }
  return single;
  }



// Work out a proper range.
int *get_range(string rng) {
  string *bits;
  int *range, i, s;
  range = ({ });
  rng = lower_case(rng);
  bits = explode(rng, ",");
#if 1
  range = sum_array(map_array(bits, #'get_sub_range));
#else
  s = sizeof(bits);
  for(i = 0; i < s; i++) {
    if (bits[i][0] == '!')
     range -= get_sub_range(bits[i][1..]);
    else range += get_sub_range(bits[i]);
    }
#endif   // 1
  return sort_array(set_of(range), #'>);
  }


// Work out a "sub-range" of messages.
int *get_sub_range(string bit) {
  int *range, i, a, b;
  range = ({ });
  a = sizeof(messages);
  switch(bit) {
    case "all":
      for(i = 0; i < a; i++)
       range += ({ i });
    break;
    case "k":
    case "kept":
      for(i = 0; i < a; i++)
       if (messages[i][FLAGS] & IF_KEPT)
        range += ({ i });
    break;
    case "m":
    case "marked":
      for(i = 0; i < a; i++)
       if (messages[i][FLAGS] & IF_MARKED)
        range += ({ i });
    break;
    case "n":
    case "new":
      for(i = 0; i < a; i++)
       if (messages[i][FLAGS] & IF_NEW)
        range += ({ i });
    break;
    case "s":
    case "stored":
      for(i = 0; i < a; i++)
       if (messages[i][FLAGS] & IF_STORED)
        range += ({ i });
    break;
    case "u":
    case "unread":
      for(i = 0; i < a; i++)
       if (messages[i][FLAGS] & IF_UNREAD)
        range += ({ i });
    break;
    case "uns":
    case "unstored":
      for(i = 0; i < a; i++)
       if ((messages[i][FLAGS] & IF_STORED) == 0)
        range += ({ i });
    break;
    default: {
      string s, t;
      switch(sscanf(bit, "%s..%s", s, t)) {
        case 2:
          b = get_single(t);
          for(i = get_single(s); i <= b; i++)
           range += ({ i });
        break;
        case 1:
        case 0:
          b = get_single(bit);
          if (b != -1) range += ({ b });
        break;
        }
      }
    break;
    }
  b = sizeof(range);
  for(i = 0; i < b; i++)
   if (range[i] < 0 || range[i] >= a) {
     write("No such message.\n");
     range = range[0..i-1] + range[i+1..];
     i--;
     b--;
     }
  return range;
  }


// This is called when we leave mail.  There are various
// things to do....
void quit_mail(string cmd, int num_mesgs) {
  int index, *range;
  range = ({ });
  for(index = 0; index < num_mesgs; index++) {
    if (messages[index][FLAGS] & IF_NEW)
     messages[index][FLAGS] -= IF_NEW;
    if (messages[index][FLAGS] & IF_MARKED)
     range += ({ index });
    }
  if (sizeof(range)) {
#if defined(X_QUICK_QUIT)
    if (cmd == "x" || AUTO_DELETE(this_player()))
#else
    if (AUTO_DELETE(this_player()))
#endif   // defined(X_QUICK_QUIT)
     quit_mail2("yes", cmd, num_mesgs);
    else {
      write("Delete marked messages ? (y/N) ");
      input_to("quit_mail2", 0, cmd, num_mesgs);
      }
    }
  else
   quit_mail2("no", cmd, num_mesgs);
  return;
  }

void quit_mail2(string str, string cmd, int num_mesgs) {
  int limit;
  str = lower_case(str);
  if (str == "y" || str == "yes")
   delete_marked_messages(),
   num_mesgs = sizeof(messages);
#if defined(SIZE_QUOTA)
  limit = SIZE_QUOTA(this_player());
  if (limit != -1) {
    int index, *range, excess;
    range = ({ });
    for(index = 0; index < num_mesgs; index++)
     if ((messages[index][FLAGS] &
          (IF_NEW | IF_UNREAD | IF_STORED)) == 0)
      range += ({ index });
    if (sizeof(range) > limit) {
      excess = sizeof(range) - limit;
#if defined(X_QUICK_QUIT)
      if (cmd == "x" || AUTO_DELETE(this_player()))
#else
      if (AUTO_DELETE(this_player()))
#endif   // defined(X_QUICK_QUIT)
       quit_mail3("yes", range, excess);
      else {
        printf("Quitting now will mean %s message%c %s deleted!\n"
               "The oldest messages will be deleted. If you want\n"
               "to keep some of them say 'no' and read 'help quota'\n"
               "Quit now ? (Y/n) ",
         excess ? to_string(excess) : "no",
         excess == 1 ? 0 : 's',
         excess == 1 ? "is" : "are");
        input_to("quit_mail3", 0, range, excess);
        }
      return;
      }
    }
#endif   // defined(SIZE_QUOTA)
  quit_mail3("yes", 0, 0);
  return;
  }

void quit_mail3(string str, int *range, int excess) {
  str = lower_case(str);
  if (str == "n" || str == "no")
   return
    menu();
  if (excess) {
    int index;

    printf("Deleting excess:\n");

//    for(index = 0; index < excess; index++) {
    index = excess;
    while(index--) {
  printf("%6s: %10s  %:-15s %:-43=s\n",
        STATUS_LINE(range[index]),
        date(messages[range[index]][DATE]),
        messages[range[index]][S_MUD] ?
         sprintf("%s@%s",
          capitalize(messages[range[index]][SNDR]),
          messages[range[index]][S_MUD]) :
         capitalize(messages[range[index]][SNDR]),
        messages[range[index]][SUBJ]);

     messages = messages[0..range[index]-1] +
                messages[range[index]+1..];
     }
    printf("Deleted %s excess message%c.\n",
     excess ? to_string(excess) : "no",
     excess == 1 ? 0 : 's');
    }
  save_object(MAIL_SAVE(MNAME(this_player())));
  (void)BIM_DIR"bimd"->set_mailer(this_player(), 0);
  return;
  }


// Delete any marked messages.
void delete_marked_messages() {
  int index, size, deleted;
  size = sizeof(messages);
  for(index = 0; index < size; index++)
   if (messages[index][FLAGS] & IF_MARKED) {
     deleted++;
     messages = messages[0..index-1] + messages[index+1..];
     index--;
     size--;
     if (index <= current_mesg && current_mesg) current_mesg--;
     }
  printf("Deleted %s marked message%c.\n",
   deleted ? to_string(deleted) : "no",
   deleted == 1 ? 0 : 's');
  return;
  }


// Support for the edit object.
#if defined(BIM_EDITOR)
void quit_edit(mapping data) {
  mapping mesg;
  string *text;
  text = data[ED_BUFF];
  mesg = data[ED_EXTRA];
#else
void TW_quit_edit(string *text, object editor, mapping mesg) {
  destruct(editor);
#endif   // defined(BIM_EDITOR)
  if (!text) write("Aborted.\n");
  else mesg[MESSAGE] = implode(text, "\n");
  send_menu(mesg);
  return;
  }

string check_dir(string file) {
  string dir;
  dir = sprintf("%s/", implode(explode(file, "/")[0..<2], "/"));
  return (file_size(dir) == -2 ? dir : 0);
  }

void receive_new_mail(mapping mesg) {
  if (current_mesg == -1) current_mesg = 0;
  messages += ({ mesg });
  return;
  }

int clean_up(int arg) {
  if (arg > 1) return 0;
  (void)BIM_DIR"bimd"->set_mailer(this_player(), 0);
  return 0;
  }
  
  
private static mapping ___mTOint = ([
  "Jan":1,"Feb":2,"Mar":3,"Apr":4,"May":5,"Jun":6,
  "Jul":7,"Aug":8,"Sep":9,"Oct":10,"Nov":11,"Dec":12
  ]);
  
varargs string date(int t) {
    string dy,mn;
    int dt,h,m,s,yr;
    sscanf(ctime(t),"%s %s %d:%d:%d %d",
       dy,mn,dt,h,m,m,s,yr);
       
    return sprintf("%02d/%02d (%2s)",
           ___mTOint[mn],dt,dy[0..1]);
}


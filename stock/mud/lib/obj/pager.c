  // More v2.0  - An attempt at a usable pager for LPmuds.
  // Bubbs@Kerovnia                             ??/Nov/92
  // Bubbs@Timewarp                             19/Dec/93

inherit "obj/ansi";

static mixed   extra;
static object  quit_ob;
static string *text,
               header,
               quit_func,
               search_patt,
               last_comm;
static int     size,
               index,
               options,
               page_length;

// Define some options.
#define QUIT_AT_END          1
#define LINE_NUMBER          2
#define PERCENT_MODE         4
#define HEADER               8
#define NEW_PAGE            16

#define DEFAULT_OPTIONS      9    // 01001
#define DEFAULT_PAGE_LENGTH 20

#define TEST(t) ((t & options) / t)

#define CTRL_L     12

void set_quit_func(mixed qo, string qf) {
  if (stringp(qo)) {
    call_other(qo, "???");
    quit_ob = find_object(qo);
    }
  if (objectp(qo)) quit_ob = qo;
  if (stringp(qf)) quit_func = qf;
  }

mixed *query_quit_func() { return ({ quit_ob, quit_func }); }

void set_options(int o) { options = o; }
int query_options(int arg) { return (arg ? TEST(arg) : options); }

void set_page_length(int p) { page_length = p; }
int query_page_length() { return page_length; }

int query_default_options() { return DEFAULT_OPTIONS; }
int query_default_page_length() { return DEFAULT_PAGE_LENGTH; }

void set_extra(mixed e) { extra = e; }
mixed query_extra() { return extra; }

void reset(status arg) {
  if (arg) return;
  options = DEFAULT_OPTIONS;
  page_length = DEFAULT_PAGE_LENGTH;
  quit_func = "more_quit";
  }

void more_options(string str) {
  string *bits;
  int opt;
  if (!str || str == "") {
    printf("quit_at_end is: %s.\nline_number is: %s.\n\
percent_mode is: %s.\nheader is: %s.\nnew_page is: %s.\n\
page_length is: %d.\nOptions value: %d.\n",
     TEST(QUIT_AT_END)  ? "on" : "off",
     TEST(LINE_NUMBER)  ? "on" : "off",
     TEST(PERCENT_MODE) ? "on" : "off",
     TEST(HEADER)       ? "on" : "off",
     TEST(NEW_PAGE)     ? "on" : "off",
     page_length,
     options);
    return;
    }
  str = lower_case(str);
  bits = explode(str, " ");
  if (!sizeof(bits)) return;
  else if (bits[0] == "qae"  || bits[0] == "quit_at_end")
   opt = QUIT_AT_END,  bits[0] = "quit_at_end";
  else if (bits[0] == "ln"   || bits[0] == "line_number")
   opt = LINE_NUMBER,  bits[0] = "line_number";
  else if (bits[0] == "pm"   || bits[0] == "percent_mode")
   opt = PERCENT_MODE, bits[0] = "percent_mode";
  else if (bits[0] == "hd"   || bits[0] == "header")
   opt = HEADER,       bits[0] = "header";
  else if (bits[0] == "np"   || bits[0] == "new_page")
   opt = NEW_PAGE,     bits[0] = "new_page";
  if (opt) {
    if (sizeof(bits) == 1)
     bits += ({ TEST(opt) ? "off" : "on" }); // toggle.
    if (bits[1] == "off" && TEST(opt))
     options -= opt;                         // turn off.
    else if (bits[1] == "on" && !TEST(opt))
     options += opt;                         // turn on.
    else {
      printf("%s is already %s.\n",
       bits[0],
       TEST(opt) ? "on" : "off");
      return;
      }
    printf("%s is now: %s.\n", bits[0], TEST(opt) ? "on" : "off");
    return;
    }
  if (sizeof(bits) && (bits[0] == "pl" || bits[0] == "page_length")) {
    page_length = (sizeof(bits) == 1 ? DEFAULT_PAGE_LENGTH : to_int(bits[1]));
    printf("page_length is now: %d.\n", page_length);
    return;
    }
  if (sizeof(bits) && (bits[0] == "opt" || bits[0] == "options")) {
    options = (sizeof(bits)) == 1 ? DEFAULT_OPTIONS : to_int(bits[1]);
    printf("options is now: %d.\n", options);
    return;
    }
  return more_options(0);
  }

int search_for(string patt) {
  string *txt;
  int ind, found;
  if (!patt || patt == "") patt = search_patt;
  if (!patt || patt == "") return -2;
  search_patt = patt;
  txt = regexp(text[index..], patt);
  if (sizeof(txt) == 0) return -1;
  return member_array(txt[0], text);
  }

void more_quit(string why) {
  (status)this_player()->clear_attrib("in_pager");
  printf("%s%s", ansi_colour("normal"), ansi_colour("standard"));
  if (quit_ob && function_exists(quit_func, quit_ob))
   catch((void)call_other(quit_ob, quit_func, why, this_object(), extra));
  return;
  }

void print_prompt() {
  if (TEST(PERCENT_MODE))
   printf("%s--More (%d%%)--%s%s ",
    ansi_colour("inverse"),
    (index * 100) / size,
    ansi_colour("normal"),
    ansi_colour("normal"));
  else
   printf("%s--More (%d/%d)--%s%s ",
    ansi_colour("inverse"),
    index,
    size,
    ansi_colour("normal"),
    ansi_colour("normal"));
  input_to("process_input");
  return;
  }

varargs void print_page(int hi_ln, string patt) {
  int max;
  if (index < 0) index = 0;
  if (index == size && TEST(QUIT_AT_END))
   return more_quit("END");
  if (index == size) return print_prompt();
  max = index + page_length;
  if (TEST(NEW_PAGE) && size > page_length) {
    index = (max > size ? size - page_length : index);
    if (index < 0) index = 0;
    printf("%s%s", cursor_position(0, 0), erase_screen(2));
    }
  max = (max > size ? size : max);
  if (TEST(HEADER) && header && index != size)
   printf("%s%|70s\n%s",
    ansi_colour("highlight"),
    sprintf("[%s]", header),
    ansi_colour("normal"));
  for(; index < max; index++) {
    string str;
    str = TEST(LINE_NUMBER) ? sprintf("%-4s", to_string(index+1)) : "";
    if (hi_ln && hi_ln - 1 == index) {
      if (!patt) patt = text[index];
      str = sprintf("%s%s%s%s",
       ansi_colour("highlight"),
       str,
       ansi_colour("normal"),
       subst(text[index],
        patt,
        sprintf("%s%s%s",
         ansi_colour("highlight"),
         patt,
         ansi_colour("normal"))));
      }
    else str = sprintf("%s%s", str, text[index]);
    printf("%s\n", str);
    }
  if (max == size && TEST(QUIT_AT_END)) more_quit("END");
  else print_prompt();
  return;
  }

void process_input(string str) {
  string com, arg, *tmp;
  int tmp_ind;
  printf("%s%c%c%79s%c",
   ansi_colour("normal"), 8, 13, "", 13);
  if (str[0] != 'r' && str[0] != 'R') last_comm = str;
  else str = last_comm;
  if (!str || str == "") str = "y";
  tmp = explode(str, " ");
  if (!tmp) tmp = ({ " ", "" });
  com = tmp[0];
  if (!com) com = "";
  arg = str[1 + strlen(com)..];
  if (arg == "") arg = com[1..];
  switch(com[0]) {
  case 'Y':
  case 'y':
    print_page();
    break;
  case 'Q':
  case 'q':
  case 'N':
  case 'n':
  case 'X':
  case 'x':
    more_quit(0);
    break;
  case 'L':
  case 'l':
    index -= page_length;
    print_page();
    break;
  case '\'':
    write(ansi_colour("highlight"));
    command(str, this_player());
    write(ansi_colour("normal"));
    print_prompt();
    break;
  case ':':
    write(ansi_colour("highlight"));
    command(str[1..], this_player());
    write(ansi_colour("normal"));
    print_prompt();
    break;
  case 'O':
  case 'o':
    printf("%s                                 %sMore Options%s\n",
     TEST(NEW_PAGE) ?
      sprintf("%s%s", cursor_position(0, 0), erase_screen(2)) :
      "",
     ansi_colour("highlight"),
     ansi_colour("normal"));
    more_options(arg);
    print_prompt();
    break;
  case 'S':
  case 's':
    tmp_ind = to_int(arg);
    if (!tmp_ind) tmp_ind = 1;
    if (tmp_ind > 0) {
      index += (page_length * tmp_ind);
      print_page();
      }
    else {
      write("Invalid number to skip.\n");
      print_prompt();
      }
    break;
  case 'U':
    index -= 2 * page_length;
    print_page();
    break;
  case 'u':
    index -= 3 * page_length / 2;
    print_page();
    break;
  case 'd':
    index -= page_length / 2;
    print_page();
    break;
  case 'D':
    index += page_length;
    print_page();
    break;
  case '+':
    tmp_ind = to_int(arg);
    if (!tmp_ind) tmp_ind = 1;
    if (tmp_ind > 0) {
      index += tmp_ind - page_length;
      print_page();
      }
    else {
      write("Invalid number to advance.\n");
      print_prompt();
      }
    break;
  case '-':
    tmp_ind = to_int(arg);
    if (!tmp_ind) tmp_ind = 1;
    if (tmp_ind > 0) {
      index -= page_length + tmp_ind;
      print_page();
      }
    else {
      write("Invalid number to advance.\n");
      print_prompt();
      }
    break;
  case 'W':
  case 'w':
    tmp_ind = to_int(arg);
    if (tmp_ind > 0 && tmp_ind <= size) {
      index = tmp_ind - (page_length / 2);
      print_page(tmp_ind);
      }
    else {
      write("Bad line number.\n");
      print_prompt();
      }
    break;
  case 'G':
  case 'g':
    tmp_ind = to_int(arg);
    if (tmp_ind > 0 && tmp_ind <= size) {
      index = tmp_ind - 1;
      print_page(tmp_ind);
      }
    else {
      write("Bad line number.\n");
      print_prompt();
      }
    break;
  case '/':
    tmp_ind = search_for(str[1..]);
    switch(tmp_ind) {
    case -2:
      write("No previous search pattern.\n");
      print_prompt();
      break;
    case -1:
      write("Pattern not found.\n");
      print_prompt();
      break;
    default:
      index = tmp_ind - (page_length / 2);
      print_page(tmp_ind + 1, search_patt);
      }
    break;
  case 'T':
  case 't':
    index = 0;
    print_page();
    break;
  case 'B':
  case 'b':
    index = size - page_length;
    print_page();
    break;
  case '?':
    printf("\
%s%s                                More Help%s\n\
<CR>, Y, y              Next page.\n\
Q, q, N, n, X, x        Quit.\n\
L, l                    Redisplay page.\n\
S, s [<num>]            Skip <num> pages. (default: 1)\n\
U (u)                   Up (half) page.\n\
-[<num>]                Up <num> lines. (default: 1)\n\
D (d)                   Down (half) page.\n\
+[<num>]                Down <num> lines. (default: 1)\n\
R, r                    Repeat last command.\n\
G, g <line_num>         Goto line <line_num>.\n\
W, w <line_num>         Print page around line <line_num>.\n\
<line_num>              Like 'W', print page around <line_num>.\n\
O, o <options>          Set options.\n\
T, t                    Goto top.\n\
B, b                    Goto bottom.\n\
/<pattern>              Case sensitive search for <pattern>.\n\
!<command>              Issue game command.\n\
'<mesg>                 Say <mesg> as with the say command.\n\
?                       This help page.\n",
     TEST(NEW_PAGE) ?
      sprintf("%s%s", cursor_position(0, 0), erase_screen(2)) :
      "",
     ansi_colour("highlight"),
     ansi_colour("normal"));
    print_prompt();
    break;
  default:
    if (to_int(com)) process_input(sprintf("w%s", com));
    else print_prompt();
    }
  return;
  }

varargs void more_text(string *txt, string head) {
  if (!txt || !pointerp(txt) || !sizeof(txt)) return;
#if 0
  if (!quit_ob   || !objectp(quit_ob) ||
      !quit_func || !stringp(quit_func))
   return;
#endif
  (void)this_player()->set_tmp_attrib("in_pager", this_object());
  header = head;
  index = 0;
  text = txt;
  size = sizeof(text);
  if(page_length == 0)
    page_length = size;
  process_input("");
  }

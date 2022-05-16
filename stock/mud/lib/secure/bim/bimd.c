// BIM: Bubbs' Intermud Mailer.
// bimd.c  :  Main daemon.
// This daemon is responsible for sending, and receiving
// mail.  It also handles redirection, and mail groups,
// both global and personal.

// And, from v1.41, it also knows which users are using mail,
// and what their mail objects are.  This is necessary, so we
// know what object to tell if a user receives mail while in
// the mail system.


#include <bim.h>


#if defined(SML_TRIMSTR)
inherit BIM_DIR"simuls";
#endif


static mapping Groups,          // The global groups.
               Mailers;         // Users and their mail objects.
static string *MDaemons;        // Array of Mail Daemons.

mapping       *messages,        // A users messages.
               mgroups;         // A users own groups.


void read_groups_from_file();


#if defined(COMPAT_FLAG)
void reset(status arg) {
  if (arg) return;
#else
void create() {
#endif   // COMPAT_FLAG
  read_groups_from_file();
  Mailers = ([ ]);
#if defined(MAIL_DAEMONS)
  MDaemons = MAIL_DAEMONS;
#endif
  return;
  }


// Read the global groups from file.
void read_groups_from_file() {
  string text, *lines;
  int index, size;
  Groups = ([ ]);
  text = read_file(GROUPS_FILE);
  if (!text) {
    LOG("Could not read groups file: "GROUPS_FILE".\n");
    return;
    }
  lines = explode(text, "\n");
  size = sizeof(lines);
  for(index = 0; index < size; index++)
   if (lines[index] != "" && lines[index][0] != '#') {
     string gname, gdefn;
     if (sscanf(lines[index], "%s:%s", gname, gdefn) == 2)
      Groups[trimstr(gname)] = map_array(explode(gdefn, ","),
                                          #'trimstr) - ({ "" });
     else
      LOG(sprintf("Bad line(%d) in groups file: "GROUPS_FILE".\n",
       index+1));
     }
  return;
  }


// Detect a valid destination.
varargs status valid_destination(string str, status quietly) {
  string error;
  int index, size;
  if (!this_player()) quietly = 1;
  if (member(Groups, str))
   return 1;
  if (USER_EXISTS(lower_case(str)))
   return 1;
#if defined(MAIL_DAEMONS)
  size = sizeof(MDaemons);
  for(index = 0; index < size; index++) {
    string err;
    err = 0;
    if ((status)MDaemons[index]->valid_destination(&str, &err))
     if (!stringp(err))
      return 1;
    if (stringp(error))
     error = err;
    }
#endif   // MAIL_DAEMONS
  if (!quietly)
   write(error ? error : sprintf("Destination not found: %s.\n", str));
  return 0;
  }


// Store the user and his mail object.
void set_mailer(object user, object mailer) {
  m_indices(Mailers);
  if (Mailers[user])
   destruct(Mailers[user]);
  if (mailer)
   Mailers[user] = mailer;
  else
   m_delete(Mailers, user);
  return;
  }


// Query a users mail object.
object query_mailer(object user) {
  return Mailers[user];
  }


// Send Mail.  Return 1 for success.
varargs status send_mail(string rcpt, mapping mesg, int flags) {
  if (!this_player())
   flags |= SF_QUIETLY;
  mesg = copy_mapping(mesg);
  if (member(Groups, rcpt)) {
    // Group send.
    mesg[MAIL_GROUP] = rcpt;
    if ((flags & SF_QUIETLY) == 0)
     printf("Sending mail to group \"%s\".\n", rcpt);
    map_array(Groups[rcpt], #'send_mail, mesg, flags);
    return 1;
    }
  else if (USER_EXISTS(rcpt)) {
    object user;
    string redir;
    // User send.
    if ((flags & SF_QUIETLY) == 0)
     printf("Sending mail to %s.\n", capitalize(rcpt));
    // Need to restore early; for redirecting to a personal mail group.
    rcpt = lower_case(rcpt);
    if (!restore_object(MAIL_SAVE(rcpt))) {
      LOG(sprintf("Failed to restore for %s.  [%O, %O]\n",
       rcpt, previous_object(), this_player()));
      messages = ({ });
      mgroups = ([ ]);
      }
    // First, check to see if there's any redirections going on.
    if ((flags & SF_NO_REDIR) == 0)
     redir = READ_REDIR(rcpt);
    if (redir) {
      redir = trimstr(redir, '\n');
      if (valid_destination(&redir, 1) || member(mgroups, redir)) {
        mesg[ORIG RCPT] = mesg[RCPT];
        mesg[RCPT] = redir;
        send_mail(redir, mesg, flags | SF_NO_REDIR);
        return 1;
        }
      }
    mesg[FLAGS] = IF_NEW | IF_UNREAD;
    if (user = find_player(rcpt)) {
      object ob;
      // If they're in mail, give the mailer object the new mail.
      if (ob = Mailers[user])
       (void)ob->receive_new_mail(mesg);
      tell_object(user,
       sprintf("You have new mail.  From %s, subject: %s.\n",
        capitalize(mesg[SNDR]), mesg[SUBJ]));
      }
    messages += ({ mesg });
    save_object(MAIL_SAVE(rcpt));
    // Save the mail too.  This will probably get overwritten
    // if they are reading mail, but not necessarily.
    messages = 0;
    mgroups = 0;
    return 1;
    }
  else {
    status sent;
#if defined(MAIL_DAEMONS)
    string error;
    int index, size;
    size = sizeof(MDaemons);
    for(index = 0; index < size && !sent; index++) {
      string err;
      err = 0;
      if ((status)MDaemons[index]->valid_destination(&rcpt, &err))
       if (!stringp(err))
        sent = (status)MDaemons[index]->send_mail(rcpt, mesg, flags);
      if (sent || stringp(err))
       error = err;
      }
    if (error && (flags & SF_QUIETLY) == 0)
     write(error);
#endif
    if (!sent)
     LOG(sprintf("bimd::send_mail(): Failed to find recipient %O.\n", rcpt));
    return sent;
    }
  return 0;
  }


// Find out where to send mail.
string *eval_groups(string dest) {
  return (member(Groups, dest) ? Groups[dest] : ({ dest }));
  }


// Get "new mail" string.
string query_new_mail(string user, status force_outp) {
  string outp;
  int index, size, new, unread;
  if (!USER_EXISTS(user)) return "No such user";
  if (!restore_object(MAIL_SAVE(user))) {
    messages = ({ });
    mgroups = ([ ]);
    }
  size = sizeof(messages);
  for(index = 0; index < size; index++) {
    if (messages[index][FLAGS] & IF_NEW) new++;
    else if (messages[index][FLAGS] & IF_UNREAD) unread++;
    }
  if (!new && !unread && !force_outp) return "";
  outp = sprintf("%s new message%c",
   new ? to_string(new) : "no",
   new == 1 ? 0 : 's');
  if (unread)
   outp = sprintf("%s and %d unread message%c",
    outp,
    unread,
    unread == 1 ? 0 : 's');
  return sprintf("%s.\n", outp);
  }


mapping query_Groups()  { return copy_mapping(Groups); }
mapping query_Mailers() { return copy_mapping(Mailers); }
string *query_MDaemons() { return MDaemons; }

#define adminp(WHO)                ( ( (WHO) -> query_security() ) >= 90 )
#define wizardp(WHO)                ( ( (WHO) -> query_security() ) >= 10 )
#define localtime(TRASH)        ( time() )
 
#include <udp.h>
 
#define COST     0     // 5
#define DO_TELL  (string)"cmds/std/tell"->do_tell
string *query_cmd_aliases() { return ({ "r" }); }
 
varargs status main(string str, status cntd, mixed reply) {
  string name;
  int wiz_level;
  status free;
    if(this_player()->query_attrib("silenced")) return 0;
  if (!cntd)
   reply = (mixed)this_player()->query_attrib("reply_to");
  if (!str) {
    if (!reply)
     return notify_fail("No one to reply to.\n"),0;
    else if (query_editing(this_player()) ||
             query_input_pending(this_player()))
     printf("Replying to %s.\n",
      capitalize(stringp(reply) ?
       reply : (string)reply->query_real_name()));
    else {
      printf("Reply to %s with what message ?\n"
             "Message: ",
       capitalize(stringp(reply) ?
        reply : (string)reply->query_real_name()));
      input_to("main", 0, 1, reply);
      }
    return 1;
    }
  if (str == "") {
    write("Aborted.\n");
    return 1;
    }
  if (!reply)
   return
    notify_fail("No one to reply to.\n"),0;
  if (stringp(reply)) {
    string mudname;
    // Must be an intermud reply!
    if (sscanf(reply, "%s@%s", name, mudname) == 2) {
      string error;
      error = (string)INETD->send_udp(mudname, ([
       REQUEST   : "tell",
       SENDER    : (string)this_player()->query_real_name(),
       RECIPIENT : lower_case(name),
       "is_reply": 1,
       DATA      : str,
       ]), 1);
      write(error ? error : "Message transmitted.\n");
      return 1;
      }
    else {
      log_file("REPLY", "reply was string with no @\n");
      return notify_fail("You've found a Bug!\n"),0;
      }
    }
  wiz_level = (int)reply->query_security();
  if (wiz_level > 10 && (int)this_player()->query_security() < wiz_level &&
   (string)reply->short() == 0) {
    // this_player() cannot see reply. Give same message as "not on".
    notify_fail("No one to reply to.\n");
    return 0;
    }
#if COST != 0
  if (wiz_level > 30 || (this_player()->query_security() > 10) ||
   (status)this_player()->query_ghost())
    free = 1;
  if (!free)
   (void)this_player()->restore_spell_points(-COST);
#endif
  write(DO_TELL(reply,
   capitalize((string)this_player()->query_real_name()), str, 1));
  return 1;
  }
 
void help() {
  write("Usage: reply [<message>]\n"
        "Reply to the last tell.  If no argument\n"
        "is given, then save who your are replying\n"
        "to, and prompt for the message.\n"
        "See also: \"tell\".\n");
  return;
  }

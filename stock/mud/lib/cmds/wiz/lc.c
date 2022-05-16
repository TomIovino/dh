// By Bubbs.
#include <ansi.h>
 
status main(string arg) {
  string *hist;
  int offset, index, size;
  object who;
 
  if(!arg || !stringp(arg))
    return notify_fail("lastcomm:  Usage:  lastcomm <name>\n"),0;
  if(!who = find_player(lower_case(arg)))
    return notify_fail(
             sprintf("lastcomm:  %O not logged on.\n",capitalize(arg))),0;
 
    if(this_player()->query_security_level() < 100
    && who->query_security_level() > this_player()->query_security_level()){
        return notify_fail(
            sprintf("lastcomm: %O not logged on.\n",capitalize(arg))),0;
    }
  hist = (string *)who->query_cmd_history();
  size = sizeof(hist);
  offset = (int)who->query_cmd_number() - size;
  printf("%s command%c in the history of %O.\n",
   size ? (string)size : "No", size == 1 ? 0 : 's', capitalize(arg));
  for(index = 0; index < size; index++)
   printf("%2d     %s" NORM "\n", index+offset, hist[index]);
  return 1;
  }
 
status help() {
  write("Usage: lastcomm <name>\n"
        "Show history of the player <name>.\n");
  return 1;
  }

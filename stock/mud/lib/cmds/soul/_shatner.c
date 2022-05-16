#include <soul.h>
 
status emote(string str) {
   object who;
   string whoobj;
 
   who = present(str,environment(this_player()));
   str = capitalize(str);
   if(!who) return 0;
   whoobj = who->query_objective();
   /* having Dreamtime's females ripping their shirts off might not be wholely appropriate, so... */
    if(this_player()->query_gender_string() == "female") {
      tell_object(who, TPNAME + " suddenly grabs you and kisses you passionately.\n");
      write("You suddenly grab " + str + " and kiss " + whoobj + " passionately.\n");
      say(TPNAME + " suddenly grabs " + str + " and kisses " + whoobj + " passionately.\n", who);
      return 1;
   }
   tell_object(who, TPNAME + " rips off " + POSS + " shirt, grabs you, and kisses you passionately.\n");
   write("You rip off your shirt, grab " + str + ", and kiss " + whoobj + " passionately.\n");
   say(TPNAME + " rips off " + POSS + " shirt, grabs " + str + ", and kisses " + whoobj + " passionately.\n", who);
   return 1;
 
}

string channel_msg(string str) {
   object who;
   string whoobj;
 
   who = find_player(lower_case(str));
   str = capitalize(str);
   if(!who) return 0;
   whoobj = who->query_objective();
    if(this_player()->query_gender_string() == "female") {
      return (TPNAME + " suddenly grabs " + str + " and kisses " + whoobj + " passionately.");
   }
   return (TPNAME + " rips off " + POSS + " shirt, grabs " + str + ", and kisses " + whoobj + " passionately.");
}
 
/* EOF */

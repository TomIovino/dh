#include <soul.h>
 
status emote(string str) {
   object who;
 
   who = present(lower_case(str), environment(TP));
   if(!who || !living(who)) {
      notify_fail("greet who ?\n");
      return 0;
   }
   str = capitalize(str);
 
   /* the following section will be modified to support races (and possibly  */
   /* guilds as well) when they are added to Dreamtime   -Sparrowhawk        */
 
   if(TP->query_security() && who->query_security()) {
      tell_object(who, TPNAME + " greets you in the ancient tongue of Wizards.\n");
      write("You greet " + str + " in the ancient tongue of Wizards.\n");
      say(TPNAME + " greets " + str + " in the ancient tongue of Wizards.\n",who);
      return 1;
   }
   tell_object(who, TPNAME + " greets you warmly.\n");
   write("You greet " + str + " warmly.\n");
   say(TPNAME + " greets " + str + " warmly.\n", who);
   return 1;
}

string channel_msg(string str) {
   object who;
 
   who = find_player(lower_case(str));
   if(!who || !living(who)) { return 0; }

   str = capitalize(str);
 
   if(TP->query_security() && who->query_security()) {
      return (TPNAME + " greets " + str + " in the ancient tongue of Wizards.");
   }
   return (TPNAME + " greets " + str + " warmly.");
}
 
/* EOF */

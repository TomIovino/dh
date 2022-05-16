#include <soul.h>
#include <levels.h>
 
status emote(string str) {
   object who;
   string type;
   int strength;
    strength = (int) this_player()->query_stat("strength");
    switch(strength) {
       default:  type = " puny";
             break;
       case 1..30:  type = " puny";
             break;
       case 31..40:  type = " weak";
             break;
       case 41..50:  type = " ";
             break;
       case 51..60:  type = " strong";
             break;
       case 61..70:  type = " bulging";
             break;
       case 71..80:  type = " steel-like";
             break;
       case 81..200:  type = " perfectly sculptured";
             break;
   }
   if(!str) {
      write("You flex your" + type + " muscles.\n");
      say(TPNAME + " flexes " + POSS + type + " muscles.\n");
      return 1;
   }
   who = present(lower_case(str), environment(TP));
   if(!who || !living(who)) {
      notify_fail("This shouldn't happen.\n");
      return 0;
   }
   str = capitalize(str);
   tell_object(who, TPNAME + " flexes " + POSS + type + " muscles for you.\n");
   write("You flex your" + type + " muscles for " + str + ".\n");
   say(TPNAME + " flexes " + POSS + type + " muscles for " + str + ".\n", who);
   return 1;
}

string channel_msg(string str) {
   object who;
   string type;
   int strength;
    strength = (int) this_player()->query_stat("strength");
    switch(strength) {
       default:  type = " puny";
             break;
       case 20..24:  type = " small";
             break;
       case 25..29:  type = " weak";
             break;
       case 30..34:  type = " ";
             break;
       case 35..39:  type = " strong";
             break;
       case 40..44:  type = " bulging";
             break;
       case 45..49:  type = " steel-like";
             break;
       case 50..101:  type = " perfectly sculptured";
             break;
   }
   if(!str) {
      return (TPNAME + " flexes " + POSS + type + " muscles.");
   }
   who = find_player(lower_case(str));
   if(!who || !living(who)) {
      return 0;
   }
   str = capitalize(str);
   return (TPNAME + " flexes " + POSS + type + " muscles for " + str);
}
 
/* EOF */

// Flip by Ash! based off flex by Gwiz, based off flex from SE. =)

#include <soul.h>
#include <levels.h>
 
status emote(string str) {
   object who;
   string type;
   int strength;
    strength = (int) this_player()->query_stat("dexterity");
    switch(strength) {
       default:  type = " ";
             break;
       case 20..24:  type = " lousy";
             break;
       case 25..29:  type = " clumsy";
             break;
       case 30..34:  type = " ";
             break;
       case 35..39:  type = " quality";
             break;
       case 40..44:  type = " gymnastic";
             break;
       case 45..49:  type = " amazing";
             break;
       case 50..101:  type = " breathtaking";
             break;
   }
   if(!str) {
      write("You perform a" + type + " flip.\n");
      say(TPNAME + " performs a" + type + " flip.\n");
      return 1;
   }
   who = present(lower_case(str), environment(TP));
   if(!who || !living(who)) {
      notify_fail("This shouldn't happen.\n");
      return 0;
   }
   str = capitalize(str);
   tell_object(who, TPNAME + " performs a" + type + " flip for you.\n");
   write("You perform a" + type + " flip for " + str + ".\n");
   say(TPNAME + " performs a" + type + " flip for " + str + ".\n", who);
   return 1;
}

string channel_msg(string str) {
   object who;
   string type;
   int dex;
    dex = (int) this_player()->query_stat("dexterity");
    switch(dex) {
       default:  type = " ";
             break;
       case 20..24:  type = " lousy";
             break;
       case 25..29:  type = " clumsy";
             break;
       case 30..34:  type = " ";
             break;
       case 35..39:  type = " quality";
             break;
       case 40..44:  type = " gymnastic";
             break;
       case 45..49:  type = " amazing";
             break;
       case 50..101:  type = " breathtaking";
             break;
   }
   if(!str) {
      return (TPNAME + " performs a" + type +" flip.");
   }
   who = find_player(lower_case(str));
   if(!who || !living(who)) {
      return 0;
   }
   str = capitalize(str);
   return (TPNAME + " performs a" + type + " flip for " + str);
}


/* EOF */

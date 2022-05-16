#include <soul.h>
 
status emote(string str) {
   object who, tp;
   if(!str) return notify_fail("ni who?");
   who = present(lower_case(str), environment(TP));
   str = capitalize(str);
   if(!who) return notify_fail("The feeling 'ni' does not support that.\n");
   if(who == TP) {
      write("You say 'Ni!' to yourself.\n");
      say(TPNAME + " says 'Ni!' to " + OBJSELF + ".\n");
   }  else {
      tell_object(who, TPNAME + " says 'Ni!' to you.\n");
      write("You say 'Ni!' to " + str + ".\n");
      say(TPNAME + " says 'Ni!' to " + str + ".\n", who);
   }
   if(living(who)) {
       tp = this_player();
       (void)"/secure/sec/stp"->set_tp(who);
       command("*wince in pain",who);
       (void)"/secure/sec/stp"->set_tp(tp);
   }
   return 1;
}

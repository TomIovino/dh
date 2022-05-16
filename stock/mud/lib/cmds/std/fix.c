#include <ansi.h>
#include <mudlib.h>
#define TP this_player()
status main(string arg) {

   this_player()->set_alt_name(this_player()->query_name());
   if(!arg){
        write("fix <what> [command logged for debugging..]\n");
        return 1;
   }
   log_file("FIX",
      this_player()->query_name()+" fixed. REASON:"+arg+" AT:"+ctime()+"\n");

    TP->recalc_hp();
    TP->recalc_wc();
    TP->recalc_ac();
    TP->recalc_carry();
    TP->cpr();
 write("LOGGED INFO.. if you still are bugged try relogging into the system.\n");
 return 1;
}

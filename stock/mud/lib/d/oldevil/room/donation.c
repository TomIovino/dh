#include <mudlib.h>
#include <ansi.h>
#include "../element.h"
inherit CHURCH;


void reset(status arg) {
    object obj;
    call_other(PATH+"/obj/CHEST","query_weight()");
    move_object(find_object(PATH+"/obj/CHEST"),this_object());
    ::reset(arg);
    if(arg) return;
    set_type("evil");
    set_short("The Donation");
    set_long(COLOR+
"This is the donation room for the needy of the world. New and old\n"+
"adventurers are welcomed to take what they need. Be warned however\n"+
"the gods dont favor the greedy.\n"+
  "");
    set_no_steal(1);
    set_no_fight(1);
    set_light(2);
    set_items(({
      }));
    set_exits(({
        PATH+"room/church","south",
      }));
   }

init(){
  ::init();
  add_action("get","take");
  add_action("get","get");
      }
int get(string what){
    string x,y;
    if(what == "all from chest")   {
    write("Something prevents that action.\n");
    return 1;    }

   if(sscanf(what, "%s from %s", x,y) == 2 && y == "chest"){
   log_file("DLOG.dat",
   this_player()->query_name()+" welfared "+x+"\n"); 
   return 0;
   }
   return 0;
}


#include <mudlib.h>
#define TPQN this_player()->query_name()
inherit ROOM;
#include <ansi.h>

reset (arg) {
  if (arg) return;
   add_object("threeteeth","/d/world/npc/threeteeth");
   set_short("dirt clearing");
   set_long(GREEN_F+
   "The trees crowd around this dirt clearing which is scorched by\n"+
   "a stone fire pit. A few broken bones lay near the firepit.\n"+
   NORM);
   set_no_track(1);
   set_exits (({
   "d/world/w/12","out",
              }));
   set_light(2);
   set_items(({ 
   }));
}

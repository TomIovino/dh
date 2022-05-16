#include <mudlib.h>
#define TPQN this_player()->query_name()
inherit ROOM;
#include <ansi.h>

reset (arg) {
  if (arg) return;
   add_object("ray","/d/world/npc/ray");
   set_short("blocked route");
   set_long(BROWN_F+
"Ancient mountains come crashing down on both sides of what is now\n"+
"an impassable route to the north. Boulders have completly filled\n"+
"in the area making hopes of travel futile.\n"
  NORM);
   set_no_track(1);
   set_exits (({
   "d/world/m/1","south",
              }));
   set_light(2);
   set_items(({ 
   }));
}

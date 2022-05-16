#include <mudlib.h>
#define TPQN this_player()->query_name()
inherit ROOM;
#include <ansi.h>

reset (arg) {
  if (arg) return;
   add_object("bubbs","/d/world/npc/bubbs");
   add_object("amylaar","/d/world/npc/amylaar");
   add_object("zihweeg","/d/world/npc/zihweeg");
   set_short("ancient ruins");
   set_long(GREEN_F+
   "A breach in the fabric of the time-space vortex.\n"+
  NORM);
   set_no_track(1);
   set_exits (({
   "d/world/etc/c9","out",
              }));
   set_light(2);
   set_items(({ 
   }));
}

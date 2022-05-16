#include <mudlib.h>
#define TPQN this_player()->query_name()
inherit ROOM;
#include <ansi.h>

reset (arg) {
  if (arg) return;
   add_object("merlin","/d/world/npc/merlin");
   set_short("circle of trees");
   set_long(GREEN_F+
  "A protective circle of trees around ancient stone ruins form a\n"+
  "hidden forest grove. Wildflowers and mushrooms speckle various\n"+
  "colors in the otherwise grassy floor of the grove.\n"+
  NORM);
   set_no_track(1);
   set_exits (({
   "d/world/g/13","out",
              }));
   set_light(2);
   set_items(({ 
   }));
}

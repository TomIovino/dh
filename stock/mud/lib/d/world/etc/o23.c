#include <mudlib.h>
inherit ROOM;
#include <ansi.h>

reset (arg) {
  if (arg) return;
   add_object("robin","/d/world/npc/robin");
   set_short("dirt clearing");
   set_long(GREEN_F+
   "The trees crowd around this dirt clearing which is marked by a\n"+
   "single stone marker which is laced and carved with runes of an\n"+
   "ancient and magical nature. Just beyond the clearing the forest\n"+
   "once again becomes the only prominent feature.\n"+
   NORM);
   set_no_track(1);
   set_exits (({
   "d/world/o/23","out",
              }));
   set_light(2);
   set_items(({ 
     "marker","Made of solid granite and very heavy.\n",
   }));
}

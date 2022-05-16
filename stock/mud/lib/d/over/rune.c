#include <ansi.h>
#include <mudlib.h>
inherit ENV;

void reset(status arg) {
object board;


  if(arg) return;
  set_short("The inner room of wizards");
  set_long(CYAN_F+
  "These portals lead to the mortal areas of interest.\n"+
  NORM);
  set_exits(({
   "w/ash/base/fire/rm1","fire",
   "w/ash/base/water/rm1","water",
   "w/ash/base/air/rm1","air",
   "w/ash/base/earth/rm1","earth",
   "w/ash/base/order/rm1","order",
   "w/ash/base/chaos/rm1","chaos",
   "w/ash/base/death/rm1","death",
   "w/ash/base/life/rm1","life",
   "d/over/portal","portal",
  })); 
  set_light(2);
}

#include <ansi.h>
#include <mudlib.h>
inherit ENV;

void reset(status arg) {
object board;


  if(arg) return;
  set_short("The inner room of wizards");
  set_long(CYAN_F+
  "These portals lead to the mortal areas of interest.\n"+
  "Take various portals to visit the weakling mortals.\n"+
  NORM);
  set_exits(({
   "d/over/inner","up",
   "d/over/rune","rune",
   "/d/town/road/mainsqr","twi",
   "/d/town/church/aboard","church",
  })); 
  set_light(2);
}

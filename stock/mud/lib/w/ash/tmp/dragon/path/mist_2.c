#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

int blah;
void reset(status arg) {
  blah = 0;
  if(arg) return;
  set_short("Mist Valley"); 
  set_long(GREY_F+
"The mist of the valley is thick and lingers everywhere distorting\n"+
"shadows and movements and causing a great darkness all about.\n"+NORM);
   set_items(({
   "sky#up",
      "Where?\n",
    "walls#down#floor#gravel#rock#granite",
      "The floor of the valley is rocky with some weeds.\n",
    }));
    set_light(1);
    set_smell("Moist and warm.\n");
    set_listen("You hear a muffled echo far off, perhaps an animal is hunting.\n");
    set_exits(({
      "/w/ash/dragon/path/mist_1","away",
      "/w/ash/dragon/path/mist_1","closer",
      "/w/ash/dragon/path/mist_1","there",
      "/w/ash/dragon/path/mist_1","here",
              }));
}




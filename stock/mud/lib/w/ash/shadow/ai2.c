// annihilate
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {

  if(arg) return; 

set_long(GREY_F+
"This tunnel consists of the same stone as before but the walls have been\n"+
"marked with the "+B_WHITE_F+
                 "white hexagons"+GREY_F+
                                " that have been engraved into the wall.\n"+ 
NORM);
   set_items(({
    "chamber#chambers#tunnels",
    "walls#wall#up#sky#down#floor",
    "Composed of ancient rock it has a very smooth surface.\n",
    "pebbles","Apparently.. from a bigger rock at one time.\n",
    "rock", "Close examination reveals a melted?? surface.\n",
   }));
    set_light(2);
    set_short("Dark Shadows");
    set_smell("Sulphur and gasses.. overall a very unpleasant smell.\n");
    set_listen("echo echo echo echo echo echo echo echo echo echo.\n");
    set_exits(({
      "/w/ash/shadow/ai1","west",
      "/w/ash/shadow/ai_n","north",
      "/w/ash/shadow/ai_s","south",
      "/w/ash/shadow/ai3","up",
              }));
}



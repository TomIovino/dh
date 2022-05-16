// annihilate
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {

  if(arg) return; 

set_long(GREY_F+
"This tunnel consists of the same stone as before but the walls have been\n"+
"marked with the "+B_RED_F+
                 "red triangles"+GREY_F+
                                " that have been engraved into the wall.\n"+ 
"Smooth stone steps lead up from here to another level of the tunnel\n"+

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
      "/w/ash/shadow/fi1","south",
      "/w/ash/shadow/fi_w","west",
      "/w/ash/shadow/fi_e","east",
      "/w/ash/shadow/fi3","up",
              }));  


}



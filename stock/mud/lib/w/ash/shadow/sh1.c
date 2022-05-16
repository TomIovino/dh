// annihilate
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

int open;
int w,e,a,f;
void reset(status arg) {

  if(arg) return; 


set_long(GREY_F+
"Stairs against the wall descend into a hemisphere shaped room of\n"+
"stone. At each of the cardinal directions is a circular tunnel that\n"+
"leads off into the darkness.\n"+
NORM);
   set_items(({
    "chamber#chambers#tunnels",
    "Above each of the tunneles is a symbol.\n"+
    "N - Triangle, W - Pentagon, E - Hexagon, S - Square.\n",
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
      "/w/ash/shadow/pit","down",
      "/w/ash/shadow/enter","up",
      "/w/ash/shadow/fi1","north",
      "/w/ash/shadow/wa1","south",
      "/w/ash/shadow/ea1","west",
      "/w/ash/shadow/ai1","east",
              }));
}



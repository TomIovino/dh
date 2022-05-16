#include <ansi.h>
#include <mudlib.h>

inherit ROOM;

void reset(status arg) {
    add_object("golem","/w/ash/shadow/npc/golem_b");
  if(arg) return; 
set_long(GREY_F+
"This is the pit of the cave network. A small set of stone steps\n"+
"has been carved into the cave wall. Small pebbles cover\n"+
"the harder surface of the ancient stone all around you.\n"+
NORM);
   set_items(({
    "tunnel", "A tunnel is smooth and worn from the ages.\n",
    "darknesn","The opposite of well lit.\n",
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
      "/w/ash/shadow/sh1","up",
              }));
}

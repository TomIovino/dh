#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "19"
#define THIS_NUMBER "20"
#define SOUTH "21"

void reset(status arg) {
  if(arg) return;
    set_short (BROWN_F+"western shore"+NORM);
    set_long(BROWN_F+
"The west gives way to a large ocean that fades into the western\n"+
"horizon. The wind is cool and blows across the dark rocky\n"+
"beach which gives way to the brushlands a bit east.\n"+
     NORM);
  set_smell("You smell the salt water from the coast.\n");
  set_listen("You can hear the crash of the waves against the shore.\n");
    set_items(({
        "sky", "The deep blue sky holds a layer of brooding clouds.",
        "sand#shore",
        "The beach goes all the way up and down to the stormy ocean.\n",
      }));
  if(WEST)   add_exit(PATH+WEST+THIS_NUMBER, "west");
  if(EAST)   add_exit(PATH+EAST+THIS_NUMBER, "east");
  if(NORTH)  add_exit(PATH+THIS_ROW+NORTH,   "north");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
}


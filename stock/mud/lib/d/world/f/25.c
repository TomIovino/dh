#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "24"
#define THIS_NUMBER "25"
#define SOUTH 0

void reset(status arg) {
  if(arg) return;
    set_short(BROWN_F+"southern shore"+NORM);
    set_long(BROWN_F+
"The south gives way to a large ocean that fades into the southern\n"+
"horizon. The wind is warm and blows across the yellow sands of\n"+
"the beach which give way to the brushes a bit north.\n"+
     NORM);
  set_smell("You smell the salt water from the coast.\n");
  set_listen("You can hear the crash of the waves against the shore.\n");
    set_items(({
        "sky", "The deep blue sky holds a layer of brooding clouds.\n",
        "sand#shore",
        "The beach goes all the way up and down to the stormy ocean.\n",
      }));


  if(EAST)   add_exit(PATH+EAST+THIS_NUMBER,"east");
  if(NORTH)  add_exit(PATH+THIS_ROW+NORTH,   "north");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
  add_exit("/w/vhorilen/port/p1.c","enter");
}


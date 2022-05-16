#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH 0
#define THIS_NUMBER "1"
#define SOUTH "2"

void reset(status arg) {
  if(arg) return;
    set_short(BLUE_F+"ocean shore"+NORM);
    set_long(BLUE_F+
"The cold peninsula ends here in the coldness of the forest. North\n"+
"and west is the dark cold ocean. South and east returns to the chilly\n"+
"northern highlands.\n"+
     NORM);
  set_light(2);
  set_smell("You smell the salt water from the coast.\n");
  set_listen("You can hear the crash of the waves against the shore.\n");
    set_items(({
        "sky", "The deep blue sky holds a layer of brooding clouds.",
        "sand#shore",
        "The beach goes all the way up and down to the stormy ocean.\n",
      }));


  if(NORTH)  add_exit(PATH+THIS_ROW+NORTH,   "north");
  if(EAST) add_exit(PATH+EAST+THIS_NUMBER,"east");
   add_exit("w/ash/base/tundra/r1","north");
}


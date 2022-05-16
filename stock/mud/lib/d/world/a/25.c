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
"The peninsula ends here into a very earthen and muddy swamp. South\n"+
"and west is the warm and misting ocean. North and east returns to the\n"+
"brushlands of the mainland.\n"+
     NORM);
  set_light(2);
  set_smell("You smell the salt water from the coast.\n");
  set_listen("You can hear the crash of the waves against the shore.\n");
    set_items(({
        "sky", "The deep blue sky holds a layer of brooding clouds.",
        "sand#shore",
        "The beach goes all the way up and down to the stormy ocean.\n",
      }));


  if(EAST)  add_exit(PATH+EAST+THIS_NUMBER,   "east");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
   add_exit("w/ash/base/swamp/r1","south");
}


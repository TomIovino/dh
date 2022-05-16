#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH 0
#define THIS_NUMBER "25"
#define SOUTH 0

void reset(status arg) {
  if(arg) return;
    set_short(YELLOW_F+"southern shore"+NORM);
    set_long(YELLOW_F+
"The peninsula ends here in a grassless and arid plain. West stretches\n"+
"back towards the great southern plains. To the east and the south\n"+
"nothing can be seen but the vast green ocean and the horizon. An ancient\n"+
"desert route follows the southern cliff of the oceanside."+
"\n"+
     NORM);
  set_light(2);
  set_smell("You smell the salt water from the coast.\n");
  set_listen("You can hear the crash of the waves against the shore.\n");
    set_items(({
        "sky", "The deep blue sky holds a layer of brooding clouds.",
        "sand#shore",
        "The beach goes all the way up and down to the stormy ocean.\n",
      }));


  if(WEST)   add_exit(PATH+WEST+THIS_NUMBER,"west");
  if(NORTH)  add_exit(PATH+THIS_ROW+NORTH,   "north");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
   add_exit("w/ash/base/desert/r1","south");
}


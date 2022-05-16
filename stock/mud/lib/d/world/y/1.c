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
    set_short(WHITE_F+"highland peninsula"+NORM);
    set_long(WHITE_F+
"The highland peninsula ends here at a cliff with a single path navigating\n"+
"around the main mountainous face. West leads into the greater portion\n"+
"of the highlands.\n"+
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
   add_exit("w/ash/base/cliff/r1","east");
}


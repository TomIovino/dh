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
"The grassy dry plains sweep across the peninsula that ends to the\n"+
"west. East leads back to the mainland while south the ocean extends\n"+
"beyond the horizon.\n"+
     NORM);
  set_smell("You smell the salt water from the coast.\n");
  set_listen("You can hear the crash of the waves against the shore.\n");
    set_items(({
        "sky", "The deep blue sky holds a layer of brooding clouds.\n",
        "sand#shore",
        "The beach goes all the way up and down to the stormy ocean.\n",
      }));


  if(EAST)   add_exit(PATH+EAST+THIS_NUMBER,"east");
  add_exit("/w/eurale/pyramid/path1","enter");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
}


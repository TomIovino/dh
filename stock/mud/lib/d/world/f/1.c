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
    set_short(BROWN_F+"northern shore"+NORM);
    set_long(BROWN_F+
"The north gives way to a large ocean that fades into the northern\n"+
"horizon. The wind is cold and blows across the white sands of\n"+
"the beach which give way to the brushes a bit south.\n"+
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
   add_exit("/w/trent/town/pass1","enter");
}


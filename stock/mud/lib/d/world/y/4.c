#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define THIS_NUMBER "4"
#define SOUTH "5"

void reset(status arg) {
  if(arg) return;
    set_short(BROWN_F+"eastern shore"+NORM);
    set_long(BROWN_F+
"The east gives way to a large ocean that fades into the eastern\n"+
"horizon. The wind is stale and hardly blows across the granite\n"+
"rocks of the beach that give way to the brushes a bit west.\n"+
     NORM);
  set_smell("You smell the salt water from the coast.\n");
  set_listen("You can hear the crash of the waves against the shore.\n");
    set_items(({
        "sky", "The deep blue sky holds a layer of brooding clouds.\n",
        "sand#shore",
        "The beach goes all the way up and down to the stormy ocean.\n",
      }));

  if(WEST)   add_exit(PATH+WEST+THIS_NUMBER, "west");
  if(EAST)   add_exit(PATH+EAST+THIS_NUMBER,"east");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
}


#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "15"
#define THIS_NUMBER "16"
#define SOUTH "17"

void reset(status arg) {
  if(arg) return;
  WORLD::set_up_room();
  if(WEST)   add_exit(PATH+WEST+THIS_NUMBER, "west");
  if(EAST)   add_exit(PATH+EAST+THIS_NUMBER, "east");
  if(NORTH)  add_exit(PATH+THIS_ROW+NORTH,   "north");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
    add_exit("/d/castle/moonstone/h","gate");
}


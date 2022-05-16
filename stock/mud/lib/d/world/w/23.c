#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "22"
#define THIS_NUMBER "23"
#define SOUTH "24"

void reset(status arg) {
  if(arg) return;
  WORLD::set_up_room();
  if(WEST)   add_exit(PATH+WEST+THIS_NUMBER, "west");
  if(NORTH)  add_exit(PATH+THIS_ROW+NORTH,   "north");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
}


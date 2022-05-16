#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "18"
#define THIS_NUMBER "19"
#define SOUTH "20"

void reset(status arg) {
  if(arg) return;
  WORLD::set_up_room();
  if(WEST)   add_exit(PATH+WEST+THIS_NUMBER, "west");
  if(EAST)   add_exit(PATH+EAST+THIS_NUMBER,"east");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
}


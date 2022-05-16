#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "8"
#define THIS_NUMBER "9"
#define SOUTH "10"

void reset(status arg) {
  if(arg) return;
  WORLD::set_up_room();
  if(EAST)   add_exit(PATH+EAST+THIS_NUMBER,"east");
  if(NORTH)  add_exit(PATH+THIS_ROW+NORTH,   "north");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
}


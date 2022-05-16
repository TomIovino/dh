#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "14"
#define THIS_NUMBER "15"
#define SOUTH "16"

void reset(status arg) {
  if(arg) return;
  WORLD::set_up_room();
  if(WEST)   add_exit(PATH+WEST+THIS_NUMBER, "west");
  if(EAST)   add_exit(PATH+EAST+THIS_NUMBER,"east");
  if(NORTH)  add_exit(PATH+THIS_ROW+NORTH,   "north");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
  add_exit("/w/sunmoonstar/forest/forest1","trail");
}


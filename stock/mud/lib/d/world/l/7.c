#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "6"
#define THIS_NUMBER "7"
#define SOUTH "8"

void reset(status arg) {
  add_object("harpy","/w/leela/world/mobs/harpy");
  if(arg) return;
  WORLD::set_up_room();
  if(WEST)   add_exit(PATH+WEST+THIS_NUMBER, "west");
  if(EAST)   add_exit(PATH+EAST+THIS_NUMBER,"east");
  if(NORTH)  add_exit(PATH+THIS_ROW+NORTH,   "north");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
}

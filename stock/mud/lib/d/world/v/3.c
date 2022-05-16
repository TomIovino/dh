#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "2"
#define THIS_NUMBER "3"
#define SOUTH "4"

void reset(status arg) {
  add_object("snake","/w/leela/world/mobs/snake");
  if(arg) return;
  WORLD::set_up_room();
  if(WEST)   add_exit(PATH+WEST+THIS_NUMBER, "west");
  if(EAST)   add_exit(PATH+EAST+THIS_NUMBER,"east");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
}


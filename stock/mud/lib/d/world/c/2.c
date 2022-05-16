#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "1"
#define THIS_NUMBER "2"
#define SOUTH "3"

void reset(status arg) {
  if(arg) return;
  WORLD::set_up_room();
  if(NORTH)  add_exit(PATH+THIS_ROW+NORTH,   "north");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
}


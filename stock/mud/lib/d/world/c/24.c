#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "23"
#define THIS_NUMBER "24"
#define SOUTH "25"

void reset(status arg) {
  if(arg) return;
  WORLD::set_up_room();
  if(NORTH)  add_exit(PATH+THIS_ROW+NORTH,   "north");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
}


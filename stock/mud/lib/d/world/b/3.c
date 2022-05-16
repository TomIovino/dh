#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "2"
#define THIS_NUMBER "3"
#define SOUTH "4"

void reset(status arg) {
  if(arg) return;
  WORLD::set_up_room();
  if(EAST)   add_exit(PATH+EAST+THIS_NUMBER,"east");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
}


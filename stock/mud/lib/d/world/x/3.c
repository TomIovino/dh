#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define THIS_NUMBER "3"
#define SOUTH "4"

void reset(status arg) {
  if(arg) return;
  WORLD::set_up_room();
  if(WEST)   add_exit(PATH+WEST+THIS_NUMBER, "west");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
}


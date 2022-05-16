#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "13"
#define THIS_NUMBER "14"
#define SOUTH "15"

void reset(status arg) {
   add_object("tree","/inherit/objs/tree");
    if(arg) return;
    set_light(2);
    set_short(BROWN_F+"outside the city walls"+NORM);
    set_long(BROWN_F+
   "Broad oak trees follow the city walls of Twilite to the northeast\n"+
   "providing shade to those on both sides of its majestic walls. The\n"+
   "great forest swallows the land in all other directions.\n"+
      NORM);

  if(WEST)   add_exit(PATH+WEST+THIS_NUMBER, "west");
  if(EAST)   add_exit(PATH+EAST+THIS_NUMBER,"east");
  if(NORTH)  add_exit(PATH+THIS_ROW+NORTH,   "north");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
}


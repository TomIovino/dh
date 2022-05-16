#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "12"
#define THIS_NUMBER "13"
#define SOUTH "14"

void reset(status arg) {
  if(arg) return;
set_short(BROWN_F+"outside the city walls"+NORM);
set_long(BROWN_F+
"A dark rock road leads west into the forest and twists out vision\n"+
"into the world beyond. The free city of Twilite and its west gates\n"+
"are east of here on a hill overlooking the forests and mountains\n"+
"to the west beyond the protective walls of the city.\n");
  set_listen("Sounds of the city are south of here.\n");
  set_smell("A wonderful mix of food and ale can be detectect.\n");
  add_exit("d/town/road/wgates","town");
  set_light(2);
  if(WEST)   add_exit(PATH+WEST+THIS_NUMBER, "west");
  //if(EAST)   add_exit(PATH+EAST+THIS_NUMBER,"east");
  if(NORTH)  add_exit(PATH+THIS_ROW+NORTH,   "north");
  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,   "south");
}


#include "row.h"
#include <mudlib.h>
#include <ansi.h>

inherit ROOM;
inherit WORLD;

#define NORTH "11"
#define THIS_NUMBER "12"
#define SOUTH "13"

void reset(status arg) {
  if(arg) return;
set_short(BROWN_F+"outside the city walls"+NORM);
set_long(BROWN_F+
"A white sand road leads north into the forest and twists out of vision \n"+
"into the world beyond. The free city of Twilite and its north gates\n"+
"are south of here on a hill overlooking the forests and mountains\n"+
"to the north beyond the protective walls of the city.\n");
  set_listen("Sounds of the city are south of here.\n");
  set_smell("A wonderful mix of food and ale can be detectect.\n");
  add_exit("d/town/road/ngates","town");
  set_light(2);
  if(WEST)   add_exit(PATH+WEST+THIS_NUMBER,  "west");
  if(EAST)   add_exit(PATH+EAST+THIS_NUMBER,  "east");
  if(NORTH)  add_exit(PATH+THIS_ROW+NORTH,    "north");
//  if(SOUTH)  add_exit(PATH+THIS_ROW+SOUTH,    "south");
}


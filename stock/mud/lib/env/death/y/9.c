#include "row.h"
#include "../opts.h"
#include <mudlib.h>
#include <ansi.h>

inherit ENV;
inherit OPTS;

#define NORTH "8"
#define THIS_NUMBER "9"
#define SOUTH "10"

void reset(status arg) {
  if(arg) return;
  OPTS::set_up_room();
  set_type(THIS_ENV);
  if(WEST)   add_exit(PATH+THIS_ENV+"/"+WEST+"/"+THIS_NUMBER, "west");
   else      add_exit(PATH+WEST_ENV+"/y/"+THIS_NUMBER,        "west");
  if(EAST)   add_exit(PATH+THIS_ENV+"/"+EAST+"/"+THIS_NUMBER, "east");
   else      add_exit(PATH+EAST_ENV+"/a/"+THIS_NUMBER,        "east");
  if(NORTH)  add_exit(PATH+THIS_ENV+"/"+THIS_ROW+"/"+NORTH,   "north");
   else      add_exit(PATH+NORTH_ENV+"/"+THIS_ROW+"/24",      "north");
  if(SOUTH)  add_exit(PATH+THIS_ENV+"/"+THIS_ROW+"/"+SOUTH,   "south");
   else      add_exit(PATH+SOUTH_ENV+"/"+THIS_ROW+"/1",       "south");
}

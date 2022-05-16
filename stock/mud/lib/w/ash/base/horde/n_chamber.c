
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    add_object("leader",PATH+"leader");
    if(arg) return;
set_long(GREY_F+
 "A large bed in the middle of the room has a quilt made with the design of\n"+
COAT_OF_ARMS+" The room is otherwise unfurnished,\n"+
"except for a single torch on the north wall. "+FIREPLACE+"\n"
"fireplace keeps the room quite comfortable.\n");
   set_items(({
   "bed","A large, wood-framed bed.\n",
   "fireplace","The fire is burning, warming the room.\n",
   "down#d#floor", ADJECTIVE+" stone floor.\n",
   "up#u#ceiling", ADJECTIVE+" stone ceiling.\n",
   "room","A room in the keep.\n",
   "quilt","A soft, comfortable quilt covers the bed.\n",
   "torch","A torch on the wall lights the room.\n",
   "banners#banner",
      COAT_OF_ARMS+NORM+"\n",
    "walls#wall",WALLS,
    }));
    set_light(2);
    set_no_track(1);
    set_no_summon(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
      PATH+"1b","north",
              }));
}
  init(){
  ::init();
   add_action("pull","pull");
   }

pull(string str){
   if(str != "torch"){ return 0;}
   write("You hear a grinding noise.\n");
   if(!door_open){
   return 1;
   }
   if(door_open){
   return 1;
   }
}



#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    add_object("caster",PATH+"caster");
    if(arg) return;
set_long(GREY_F+
"This great room is undecorated except for "+FIREPLACE+"\n"
"firepit which keeps the room well heated. Above the firepit is a huge\n"+
"banner of "+COAT_OF_ARMS+"\n");
   set_items(({
   "firepit","The fire is burning, warming the room.\n",
   "down#d#floor", ADJECTIVE+" stone floor.\n",
   "up#u#ceiling", ADJECTIVE+" stone ceiling.\n",
   "banners#banner",
      COAT_OF_ARMS+NORM+"\n",
    "walls#wall",WALLS,
    "room","A room in the keep.\n",
    "keep","A small stone fortress.\n",
    }));
    set_no_track(1);
    set_no_summon(1);
    set_light(2);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
      PATH+"3b","south",
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


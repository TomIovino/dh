
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
   add_object("guard",PATH+"guard");
    if(arg) return;
    set_long(GREY_F+
ADJECTIVE+ " keep walls form a "+LIGHT+" lit hallway\n"+
"with the wall banners of "+COAT_OF_ARMS+"\n"
"The hallway continues to run north and south.\n"+
  "");
    set_items(({
    "walls#wall",WALLS,
    "up#u#ceiling", ADJECTIVE+" stone ceiling.\n",
    "down#d#floor", ADJECTIVE+" stone floor.\n",
    "banners#banner","Torch light shines on the banners of "+
    COAT_OF_ARMS+NORM+"\n",
    "torch","It hangs on the inside wall.\n",
    "hallway","A passage in the keep.\n",
    "keep","A small stone fortress.\n",
    }));

    set_light(2);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"1a","north",
    PATH+"3a","south",
    PATH+"w_chamber","east",
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



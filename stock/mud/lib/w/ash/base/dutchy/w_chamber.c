
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    add_object("cook",PATH+"cook");
    if(arg) return;
set_long(GREY_F+
FIREPLACE+" hearth is the centerpiece for a collection of pots\n"+
"and pans which hangs on the wall. A large wooden table sits in the middle of\n"+
"the room. A single large banner hangs above the hearth.\n"NORM); 
   set_items(({
   "hearth#centerpiece", FIREPLACE+" fireplace.\n",
   "pots#pans#collection","They don't look overly clean.\n",
   "table","A large wooden table for dining.\n",
   "down#d#floor", ADJECTIVE+" stone floor.\n",
   "up#u#ceiling", ADJECTIVE+" stone ceiling.\n",
   "room","A room in the keep.\n",
   "keep","A small stone fortress.\n",
   "banners#banner",COAT_OF_ARMS+NORM+"\n",
    "walls#wall",WALLS,
    }));
    set_light(2);
    set_no_track(1);
    set_no_summon(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
       PATH+"2a","west",
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


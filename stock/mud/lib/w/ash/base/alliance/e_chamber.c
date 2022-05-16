
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    add_object("merchant",PATH+"merchant");
    if(arg) return;
set_long(GREY_F+
ADJECTIVE+" stone table of great size sits in the middle of\n"
"this unfurnished room. A single large "+
GREEN_F"m"BROWN_F+"a"RED_F"p"GREY_F+" hangs on the far wall.\n"NORM);
   set_items(({
   "table","A large table with wooden chairs all around it.\n",
   "chairs","Large wooden chairs around the table.\n",
   "down#d#floor", ADJECTIVE+" stone floor.\n",
   "up#u#ceiling", ADJECTIVE+" stone ceiling.\n",
   "banners#banner",
   COAT_OF_ARMS+NORM,
   "walls#wall",WALLS,
   "keep","A small stone fortress.\n",
   "room","A room in the keep.\n",
   "map",GREY_F"#####\n#"COLOR"   "GREY_F"#\n"GREY_F"#"+COLOR+" "+
   COLOR+BOLD"*"COLOR" "+
   GREY_F"#\n"GREY_F"#"COLOR"   "GREY_F"#\n#####",
    }));
    set_no_track(1);
    set_no_summon(1);
    set_light(2);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
       PATH+"2c","east",
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


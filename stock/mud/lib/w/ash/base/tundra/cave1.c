
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(BLUE_F+
"Centries of ice have created this natural subteraining cave.\n"+
"Looking north deeper into the cave fear washes over you.\n"+
"West leads to the safety of being outside this place.\n"+
NORM);
    set_items(({
    "cave","Its dark.. and much darker north.\n",
    "ice#snow","Standard elements of a tundra cave.\n",
    }));
    set_light(1);
    set_door_path(PATH+"2c");
    set_door_name("west");
    set_short("chamber");
    set_smell("sniff.. decomposition?\n");
    set_listen("perhaps thats snoring?\n");
    set_exits(({
    PATH+"cave2","north",
              }));
}

init(){
  ::init();
  add_action("secret","open");
  add_action("secret","move");
}

secret(str){
  if(str != "rock") return 0;
  if(!door_open){
   command("open west",this_player());
   return 1;
   }
   if(door_open){
   command("close west",this_player());
   return 1;
  }
}


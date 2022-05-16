#include <ansi.h>
#include <mudlib.h>
inherit TREASURE;

reset(arg){
 ::reset(arg);
 call_out("dest",60);
 if(arg)return;
 set_name("portal");
 set_short("");
 set_long("\
A shimmering portal that leads somewhere.\n");
 set_weight(10000);
 set_extra_long(RED_F+"a "+BOLD+"shimmering "+NORM+RED_F+"portal hovers in the air.\n"+NORM);
 }
init(){
 ::init();
 if(present("pkey",this_player())){
   say(BOLD+RED_F+"The portal collapses in a large flash.\n"+NORM);
   destruct(this_object());
   return 1;
   }
 add_action("enter","enter");
 }
enter(str){
 if(str!="portal"){
   return 0;
   }
write(BOLD+BLUE_F+"You fail to enter the portal.\n"+NORM);
return 1;
 }
dest(){
  tell_room(this_object(),BOLD+RED_F+"The portal vanishes in a bright flash.\n"+NORM);
  destruct(this_object());
  return 1;
  }

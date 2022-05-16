#include <ansi.h>
#include <mudlib.h>
inherit TREASURE;

int open;

reset (arg){
    if(arg) return;
    set_weight(1);
    set_value(101);
    set_name("vortex");
    set_long("This is a magical travel vortex when you enter into it\n"+
             "you will be transfered to another location.\n");
}

init(){
 add_action("enter","enter");
}

enter(string what){
  if(!what == "vortex"){
    return 0;
  }
  if(this_player()->query_position() != "standing"){
      write("You must stand to walk through the vortex.\n");
      return 1;
  }
  write("You feel your self moving.. with out moving..\n");
  write("You feel dizzy and find yourself in another place.\n");
  say(this_player()->query_alt_name()+
      " enters the vortex and vanishes.\n");
  if(this_player()->query_current_room() != "d/town/church/church"){
    this_player()->move_player("X#/d/town/church/church");
    this_player()->adj_mp(-25);
  } else {
    this_player()->church();
    this_player()->adj_mp(-25);
  }
  say(this_player()->query_alt_name()+
      " appears from the vortex.\n");
  return 1;
}


query_weight(){ return 1000; }

short(){
return 
GREY_F+
"An ancient vortex floats in midair.";
}
long(){
 write(B_GREEN_F+
 "You look at the stone ring which hovers here. Its diameter is big\n"+
 "enough for a person to comfortably walk through it. \n");
 write(BLINK+"The vortex is open.\n"+NORM);
  return 1;
}

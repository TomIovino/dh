#include <mudlib.h>
#include <ansi.h>
inherit TREASURE;
reset(arg) {
    if (arg) return;
    set_name("bunny");
    set_alt_name("rabbit");
    set_short(B_WHITE_F+"fluffy white bunny rabbit"+NORM);
    set_long(
      "A fluffy white bunny rabbit covered with fluffy white fur.\n");
    set_weight(1);
    set_value(100);
}
  init(){
   ::init();
  add_action("pet","pet");
   }
string pet(string what){
    if(what != "bunny"){
     return "";
    }
   say(this_player()->query_name()+" pets the bunny and it hops around.\n");
   write("You pet the bunny and it hops around.\n");
  return "";
}

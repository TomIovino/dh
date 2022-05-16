#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("potion");
  set_alias("grey");
  set_light(1);
  set_short(
  GREY_F+"speckled "NORM+B_WHITE_F+"grey"BOLD" potion"+NORM);
  set_long(WHITE_F"\
  Speckled Grey Potion.\n");
  set_info("GREY POTION OF DEFENDER AND AVENGER.\n");
  set_no_save(0);
  set_weight(1); 
  set_value(9999);
}

init(){
 ::init();
 add_action("quaff","quaff");
 add_action("quaff","drink");
 }


quaff(string what){
  if(what != query_name() && what != query_alias()) return 0;
  write("You quaff the "+query_short()+".\n");
  say(this_player()->query_alt_name()+" quaffs the "+query_short()+".\n");
  this_player()->set_var("defender",100);
  this_player()->set_var("defender_time",3600);
  this_player()->set_var("avenger",100);
  this_player()->set_var("avenger_time",3600);
  write("You feel different.\n");
  destruct(this_object());
  return 1;
}




#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("potion");
  set_alias("green");
  set_light(1);
  set_short(
  GREEN_F+"sparkling "BOLD"green"NORM+GREEN_F" potion"+NORM);
  set_long(WHITE_F"\
  Sperkling Green Potion.\n");
  set_info("GREEN POTION OF REFRESH AND FLY.\n");
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
  this_player()->adj_mp(1000);
  this_player()->set_var("wings",100);
  this_player()->set_var("wings_time",3600);
  write("You feel refreshed and are flying.\n");
  destruct(this_object());
  return 1;
}




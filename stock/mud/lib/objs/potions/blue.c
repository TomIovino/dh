#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("potion");
  set_alias("blue");
  set_light(1);
  set_short(
  BLUE_F+"swirling "BOLD"blue"NORM+BLUE_F" potion"+NORM);
  set_long(WHITE_F"\
  Swirling Blue Potion.\n");
  set_info("BLUE POTION OF MANA AND REFLECT.\n");
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
  this_player()->adj_sp(1000);
  this_player()->set_var("reflect",100);
  this_player()->set_var("reflect_time",3600);
  write("Your mana is restored and You are shielded by a reflect spell.\n");
  destruct(this_object());
  return 1;
}




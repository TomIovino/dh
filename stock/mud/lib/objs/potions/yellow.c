#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("potion");
  set_alias("yellow");
  set_light(1);
  set_short(
  YELLOW_F+"shiny yellow potion"+NORM);
  set_long(WHITE_F"\
  Shiny yellow potion.\n");
  set_info("YELLOW POTION OF HEROISM AND BLESS.\n");
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
  this_player()->set_var("heroism",100);
  this_player()->set_var("heroism_time",3600);
  this_player()->set_var("bless",100);
  this_player()->set_var("bless_time",3600);
  write("You feel heroic and are blessed.\n");
  destruct(this_object());
  return 1;
}




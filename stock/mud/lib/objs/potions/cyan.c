#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("potion");
  set_alias("cyan");
  set_light(1);
  set_short(
  B_CYAN_F+"glowing "NORM+CYAN_F+"cyan"BOLD" potion"+NORM);
  set_long(WHITE_F"\
  Glowing Cyan Potion.\n");
  set_info("CYAN POTION OF THE BEAST CALLER!\n");
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
  this_player()->set_var("frenzy",100);
  this_player()->set_var("frenzy_time",1800);
  this_player()->set_var("claws",100);
  this_player()->set_var("claws_time",1800);
  this_player()->set_var("wings",100);
  this_player()->set_var("wings_time",1800);
  this_player()->set_var("fangs",100);
  this_player()->set_var("fangs_time",1800);
  this_player()->set_var("scales",100);
  this_player()->set_var("scales_time",1800);
  this_player()->set_var("sight",100);
  this_player()->set_var("sight_time",1800);
  write("You feel the beast in your tummy! GRRR.\n");
  destruct(this_object());
  return 1;
}




#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("potion");
  set_alias("magenta");
  set_light(1);
  set_short(
  B_MAGENTA_F+"speckled "NORM+MAGENTA_F+"magenta"BOLD" potion"+NORM);
  set_long(WHITE_F"\
  Speckled Magenta Potion.\n");
  set_info("MAGENTA POTION OF REGENERATE AND FRENZY.\n");
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
  this_player()->set_var("regenerate",100);
  this_player()->set_var("regenerate_time",3600);
  this_player()->set_var("frenzy",100);
  this_player()->set_var("frenzy_time",3600);
  write("You feel altered.\n");
  destruct(this_object());
  return 1;
}




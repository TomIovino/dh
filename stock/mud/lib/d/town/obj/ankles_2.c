#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("ankleguard");
  set_alias("ankleguards");
  set_short(B_WHITE_F"silver chainmail ankleguard"+NORM);
  set_long("Ankleguards forged from silver.\n");
  set_type("ankles");
  set_ac(5);
  set_weight(5); 
  set_value(2000);
}


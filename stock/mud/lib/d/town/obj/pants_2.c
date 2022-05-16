#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("leggings");
  set_short(B_WHITE_F"silver chainmail leggings"+NORM);
  set_long("Chainmail leggings forged from silver.\n");
  set_type("pants");
  set_ac(5);
  set_weight(5); 
  set_value(2000);
}


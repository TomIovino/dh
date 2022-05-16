#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("chainmail");
  set_alias("armour");
  set_short(B_WHITE_F"silver chainmail armour"+NORM);
  set_long("A suit of chainmail forged from silver.\n");
  set_type("armour");
  set_ac(10);
  set_weight(10); 
  set_value(4000);
}


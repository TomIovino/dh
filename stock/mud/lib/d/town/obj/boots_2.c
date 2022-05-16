#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("boots");
  set_short(B_WHITE_F"silver chainmail boots"+NORM);
  set_long("Chainmail boots forged from silver.\n");
  set_type("boots");
  set_ac(5);
  set_weight(5); 
  set_value(2000);
}


#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("sleeves");
  set_short(B_WHITE_F"silver chainmail sleeves"+NORM);
  set_long("Chainmail sleeves forged from silver.\n");
  set_type("sleeves");
  set_ac(5);
  set_weight(6); 
  set_value(250);
}


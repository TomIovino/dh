#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("wristguard");
  set_alias("wristguards");
  set_short(B_WHITE_F"silver chainmail wristguard"+NORM);
  set_long("Chainmail wristguards forged from silver.\n");
  set_type("wrists");
  set_ac(5);
  set_weight(6); 
  set_value(250);
}


#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("gloves");
  set_short(B_WHITE_F"silver chainmail gloves"+NORM);
  set_long("Chainmail gloves forged from silver.\n");
  set_type("gloves");
  set_ac(5);
  set_weight(5); 
  set_value(2000);
}


#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("sleeves");
  set_short(BROWN_F+"soft leather sleeves"+NORM);
  set_long(BROWN_F+
  "Leather sleeves constructed of leather braided together to increase\n"+
  "the amount of protection provided to the wearer.\n");
  set_type("sleeves");
  set_ac(2);
  set_weight(3); 
  set_value(10);
}


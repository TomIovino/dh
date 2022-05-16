#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("shoes");
  set_short(BROWN_F+"soft leather shoes"+NORM);
  set_long(BROWN_F+
  "Comfortable shoes of sewn together pieces of soft leather.\n");
  set_type("boots");
  set_ac(4);
  set_weight(4); 
  set_value(1000);
}


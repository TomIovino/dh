#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("cloak");
  set_short(BROWN_F+"soft leather cloak"+NORM);
  set_long(BROWN_F+
  "A simple leather traveler's cloak.\n");
  set_type("cloak");
  set_ac(4);
  set_weight(4); 
  set_value(1000);
}


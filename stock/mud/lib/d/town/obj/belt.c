#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("belt");
  set_short(BROWN_F+"soft leather belt"+NORM);
  set_long(BROWN_F+
  "A simple brown leather belt and wooden buckle.\n");
  set_type("belt");
  set_ac(4);
  set_weight(4); 
  set_value(1000);
}


#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("shield");
  set_short(BROWN_F+"copper shield"+NORM);
  set_long(BROWN_F+
  "A copper shield polished smooth.\n");
  set_type("shield");
  set_ac(4);
  set_weight(4); 
  set_value(20);
}


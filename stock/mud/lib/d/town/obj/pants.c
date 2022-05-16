#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("leggings");
  set_short(BROWN_F+"soft leather leggings"+NORM);
  set_long(BROWN_F+
"Comfortable leggings made of soft leather sewn together.\n");
  set_type("pants");
  set_ac(4);
  set_weight(4); 
  set_value(1000);
}


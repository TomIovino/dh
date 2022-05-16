#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("ring");
  set_short(BROWN_F"copper ring"+NORM);
  set_long(BROWN_F+
  "A simple ring made from copper.\n");
  set_type("ring");
  set_ac(2);
  set_weight(3); 
  set_value(10);
}


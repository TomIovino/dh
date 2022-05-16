#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("tunic");
  set_short(BROWN_F"soft leather tunic"+NORM);
  set_long(BROWN_F+
  "A comfortable soft tunic constructed of leather.\n");
  set_type("armour");
  set_ac(4);
  set_weight(4); 
  set_value(2000);
}


#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("sphere");
  set_short(B_WHITE_F"silver sphere"+NORM);
  set_light(1);
  set_long("A sphere made of silver that gives off magical light.\n");
  set_type("floating");
  set_ac(5);
  set_weight(3); 
  set_value(2000);
}


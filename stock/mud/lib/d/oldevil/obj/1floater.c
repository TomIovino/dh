#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+"tiny "+METAL+" "+TOTEM+"");
  set_short(COLOR+"tiny "+METAL+" "+TOTEM+"");
  set_type("floating");
  set_name("totem");
  set_alignment(ELEMENT);
  set_ac(2);
  set_weight(2); 
  set_value(50); 
}


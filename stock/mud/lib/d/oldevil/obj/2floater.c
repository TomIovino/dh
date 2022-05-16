#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+"small "+METAL+" "+TOTEM+"");
  set_short(COLOR+"small "+METAL+" "+TOTEM+"");
  set_type("floating");
  set_name("totem");
  set_alignment(ELEMENT);
  set_ac(5);
  set_weight(5); 
  set_value(1500); 
}


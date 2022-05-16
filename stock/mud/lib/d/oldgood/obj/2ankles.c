#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+"polished "+METAL+" "+TOTEM+" anklet");
  set_short(COLOR+"polished "+METAL+" "+TOTEM+" anklet");
  set_type("ankles");
  set_name("anklet");
  set_alignment(ELEMENT);
  set_ac(5);
  set_weight(5); 
  set_value(1500); 
}


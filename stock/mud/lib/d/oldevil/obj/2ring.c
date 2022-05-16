#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+"polished "+METAL+" "+TOTEM+" ring");
  set_short(COLOR+"polished "+METAL+" "+TOTEM+" ring");
  set_type("ring");
  set_name("ring");
  set_alignment(ELEMENT);
  set_ac(2);
  set_weight(2); 
  set_value(50); 
}


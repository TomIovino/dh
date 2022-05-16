#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+GEM+" studded "+METAL+" "+TOTEM+" anklet");
  set_long(COLOR+"A "+GEM+" studded "+METAL+" "+TOTEM+" anklet.");
  set_type("ankles");
  set_name("anklet");
  set_alignment(ELEMENT);
  set_ac(10);
  set_weight(10); 
  set_value(4000); 
}


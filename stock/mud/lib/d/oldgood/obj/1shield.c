#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+METAL+" "+TOTEM+" adorned shield");
  set_short(COLOR+METAL+" "+TOTEM+" adorned shield");
  set_type("shield");
  set_name("shield");
  set_alignment(ELEMENT);
  set_ac(5);
  set_weight(5); 
  set_value(50); 
}


#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+METAL+" "+TOTEM+" adorned leather tunic");
  set_short(COLOR+METAL+" "+TOTEM+" adorned leather tunic");
  set_type("armour");
  set_name("tunic");
  set_alignment(ELEMENT);
  set_ac(5);
  set_weight(5); 
  set_value(50); 
}


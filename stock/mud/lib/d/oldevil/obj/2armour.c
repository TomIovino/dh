#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+METAL+" "+TOTEM+" decorated chainmail");
  set_short(COLOR+METAL+" "+TOTEM+" decorated chainmail");
  set_type("armour");
  set_name("chainmail");
  set_alignment(ELEMENT);
  set_ac(15);
  set_weight(15); 
  set_value(1500); 
}


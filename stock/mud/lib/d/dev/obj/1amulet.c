#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+METAL+" "+TOTEM+" necklace");
  set_short(COLOR+METAL+" "+TOTEM+" necklace");
  set_type("amulet");
  set_name("amulet");
  set_alignment(ELEMENT);
  set_ac(2);
  set_weight(2); 
  set_value(50); 
}


#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+"polished "METAL+" "+TOTEM+" necklace");
  set_short(COLOR+"polished "METAL+" "+TOTEM+" necklace");
  set_type("amulet");
  set_name("necklace");
  set_alignment(ELEMENT);
  set_ac(5);
  set_weight(5); 
  set_value(1500); 
}


#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+GEM+" adorned "METAL+" "+TOTEM+" amulet");
  set_short(COLOR+GEM+" adorned "METAL+" "+TOTEM+" amulet");
  set_type("amulet");
  set_name("amulet");
  set_alignment(ELEMENT);
  set_ac(10);
  set_weight(10); 
  set_value(4000); 
}


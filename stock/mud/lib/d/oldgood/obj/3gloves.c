#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+GEM+" studded "+METAL+" plate gauntlets");
  set_long(COLOR+GEM+" studded"+METAL+" plate gauntlets.");
  set_type("gloves");
  set_name("gauntlets");
  set_alignment(ELEMENT);
  set_ac(5);
  set_weight(5); 
  set_value(1500); 
}


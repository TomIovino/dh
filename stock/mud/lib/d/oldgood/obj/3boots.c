#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+GEM+" studded "+METAL+" plate boots");
  set_long(COLOR+GEM+" studded"+METAL+" plate boots.");
  set_type("boots");
  set_name("boots");
  set_alignment(ELEMENT);
  set_ac(10);
  set_weight(10); 
  set_value(4000); 
}


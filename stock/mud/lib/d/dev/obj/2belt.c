#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+METAL+" chain belt and "+TOTEM+" buckle");
  set_long(COLOR+METAL+" chain belt and "+TOTEM+" buckle");
  set_type("belt");
  set_name("belt");
  set_alignment(ELEMENT);
  set_ac(5);
  set_weight(5); 
  set_value(1500); 
}


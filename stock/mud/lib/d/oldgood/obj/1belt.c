#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+"leather belt and "+METAL+" "+TOTEM+" buckle");
  set_long(COLOR+"Leather belt and "+METAL+" "+TOTEM+" buckle");
  set_type("belt");
  set_name("belt");
  set_alignment(ELEMENT);
  set_ac(2);
  set_weight(2); 
  set_value(50); 
}


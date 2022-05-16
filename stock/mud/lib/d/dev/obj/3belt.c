#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+"silk sash with "+GEM+" "+TOTEM+" buckle");
  set_long(COLOR+"Silk sash with "+GEM+" "+TOTEM+" buckle.");
  set_type("belt");
  set_name("sash");
  set_alignment(ELEMENT);
  set_ac(10);
  set_weight(10); 
  set_value(4000); 
}


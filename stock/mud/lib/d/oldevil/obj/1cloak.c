#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+"leather cloak and "+METAL+" "+TOTEM+" clasp");
  set_long(COLOR+"Leather cloak and "+METAL+" "+TOTEM+" clasp");
  set_type("cloak");
  set_name("cloak");
  set_alignment(ELEMENT);
  set_ac(2);
  set_weight(2); 
  set_value(50); 
}


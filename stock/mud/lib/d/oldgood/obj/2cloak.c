#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+METAL+" chain cloak and "+TOTEM+" clasp");
  set_long(COLOR+METAL+" chain cloak and "+TOTEM+" clasp");
  set_type("cloak");
  set_name("cloak");
  set_alignment(ELEMENT);
  set_ac(5);
  set_weight(5); 
  set_value(1500); 
}


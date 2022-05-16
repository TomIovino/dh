#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+METAL+" chain vambrances");
  set_long(COLOR+METAL+" chain vambrances.");
  set_type("sleeves");
  set_name("vambrances");
  set_alignment(ELEMENT);
  set_ac(2);
  set_weight(2); 
  set_value(50); 
}


#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+"leather sleeves");
//  set_short(COLOR+"leather pauldrun");
  set_long(COLOR+"Leather vambraces.");
  set_type("sleeves");
  set_name("vambraces");
  set_alignment(ELEMENT);
  set_ac(2);
  set_weight(2); 
  set_value(50); 
}


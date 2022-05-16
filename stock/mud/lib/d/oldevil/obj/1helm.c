#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+METAL+" "+TOTEM+" adorned cap");
  set_short(COLOR+METAL+" "+TOTEM+" adorned cap");
  set_type("helm");
  set_name("cap");
  set_alignment(ELEMENT);
  set_ac(1);
  set_weight(1); 
  set_value(50); 
}


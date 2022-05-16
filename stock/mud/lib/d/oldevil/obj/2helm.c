#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+METAL+" "+TOTEM+" decorated helm");
  set_short(COLOR+METAL+" "+TOTEM+" decorated helm");
  set_type("helm");
  set_name("helm");
  set_alignment(ELEMENT);
  set_ac(1);
  set_weight(1); 
  set_value(50); 
}


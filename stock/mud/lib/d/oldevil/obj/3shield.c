#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+GEM+" studded "+METAL+" "+TOTEM+" plate shield");
  set_long(COLOR+"A "+GEM+" studded "+METAL+" "+TOTEM+" plate shield");
  set_type("shield");
  set_name("plateshield");
  set_alignment(ELEMENT);
  set_ac(20);
  set_weight(20); 
  set_value(4000); 
}


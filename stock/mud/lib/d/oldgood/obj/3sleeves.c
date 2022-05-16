#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+GEM+" studded "+METAL+" plate vambrances");
  set_long(COLOR+GEM+" studded"+METAL+" plate vambrances.");
  set_type("sleeves");
  set_name("vambrances");
  set_alignment(ELEMENT);
  set_ac(10);
  set_weight(10); 
  set_value(4000); 
}


#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_short(COLOR+"silk cape with "+GEM+" "+TOTEM+" clasp");
  set_long(COLOR+"Silk cape with "+GEM+" "+TOTEM+" clasp.");
  set_type("cloak");
  set_name("cape");
  set_alignment(ELEMENT);
  set_ac(10);
  set_weight(10); 
  set_value(4000); 
}


#include <ansi.h>
#include <mudlib.h>
#include "path.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("greaves");
  set_short(
  ANC" "ADV" greaves"NORM);
  set_long(CO+
"Made in the old ways before the time of the racewars this item\n"+
"now carries the enchantment of "ELE+NORM+"\n");
  set_info("This item has been affected by Dragon Magic.\n");
  set_type("pants");
  set_ac(12);  
  BONUS;
  set_weight(12);
  set_value(2000);
  set_ego(75);
}


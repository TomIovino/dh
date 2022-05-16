#include <ansi.h>
#include <mudlib.h>
#include "path.h"

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("shield");
  set_short(
  ANC" "ADV" shield"NORM);
  set_long(CO+
"Made in the old ways before the time of the racewars this item\n"+
"now carries the enchantment of "ELE+NORM+"\n");
  set_info("This item has been affected by Dragon Magic.\n");
  set_type("shield");
  set_ac(20);  
  BONUS;
  set_weight(20);
  set_value(2000);
  set_ego(75);
}


#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("starshine");
  set_alias("ankleguards");
  set_short(B_BLUE_F"illuminated ankleguards with "+
  BOLD+MAGENTA_F+"j"+BLUE_F+"e"+BROWN_F+"w"+GREY_F"e"+RED_F+"l"+
  WHITE_F"e"CYAN_F"d " +B_BLUE_F+"design"+NORM);
  set_long(B_WHITE_F+
"Made in the old ways before the time of the racewars this item\n"+
"now carries a major enchantment from the Realm of Order.\n");
  set_info("This item has been infused with Dragon Magic.\n"+NORM);
  set_alignment("order");
  set_type("ankles");
  set_ac(15);  
  set_apb(3);
  set_weight(15);
  set_value(2000);
  set_ego(175);
}


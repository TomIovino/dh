#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("starshine");
  set_alias("anklet");
  set_short(YELLOW_F"shining anklet with "+
  BOLD+MAGENTA_F+"j"+BLUE_F+"e"+BROWN_F+"w"+GREY_F"e"+RED_F+"l"+
  WHITE_F"e"CYAN_F"d " +YELLOW_F+"reflections"+NORM);
  set_long(GREY_F+
"Made in the old ways before the time of the racewars this item\n"+
"now carries a major enchantement from the Realm of Order.\n");
  set_info("This item has been affected by Dragon Magic.\n"+NORM);
  set_alignment("order");
  set_type("ankles");
  set_ac(15);  
  set_apb(3);
  set_weight(15);
  set_value(2000);
  set_ego(160);
}


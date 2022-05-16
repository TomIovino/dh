#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("lifeguard");
  set_alias("wristguards");
  set_short(B_WHITE_F"shimmering wristguards with "+
  BOLD+MAGENTA_F+"j"+BLUE_F+"e"+BROWN_F+"w"+GREY_F"e"+RED_F+"l"+
  WHITE_F"e"CYAN_F"d " +B_WHITE_F+"ankh"+NORM);
  set_long(B_WHITE_F+
"Made in the old ways before the time of the racewars this item\n"+
"now carries a major enchantment from the Realm of Life.\n");
  set_info("This item has been infused with Dragon Magic.\n"+NORM);
  set_alignment("life");
  set_type("wrists");
  set_ac(15);  
  set_dpb(3);
  set_weight(15);
  set_value(2000);
  set_ego(175);
}


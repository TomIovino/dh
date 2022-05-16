#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("boneguard");
  set_alias("pauldrons");
  set_short(GREY_F"bone plated pauldrons with "+
  BOLD+MAGENTA_F+"j"+BLUE_F+"e"+BROWN_F+"w"+GREY_F"e"+RED_F+"l"+
  WHITE_F"e"CYAN_F"d " +GREY_F+"skull"+NORM);
  set_long(GREY_F+
"Made in the old ways before the time of the racewars this item\n"+
"now carries a major enchantment from the Realm of Death.\n");
  set_info("This item has been affected by Dragon Magic.\n"+NORM);
  set_alignment("death");
  set_type("sleeves");
  set_ac(15);  
  set_dpb(3);
  set_weight(15);
  set_value(2000);
  set_ego(175);
}


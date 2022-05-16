#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("dejavu");
  set_alias("mask");
  set_short(B_RED_F"red mask with "+
  BOLD+MAGENTA_F+"j"+BLUE_F+"e"+BROWN_F+"w"+GREY_F"e"+RED_F+"l"+
  WHITE_F"e"CYAN_F"d " +B_RED_F+"evil smile"+NORM);
  set_long(B_WHITE_F+
"Made in the old ways before the time of the racewars this item\n"+
"now carries a major enchantment from the Realm of Chaos.\n");
  set_info("This item has been infused with Dragon Magic.\n"+NORM);
  set_alignment("chaos");
  set_type("mask");
  set_ac(15);  
  set_apb(3);
  set_weight(15);
  set_value(2000);
  set_ego(175);
}


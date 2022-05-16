#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("vambraces");
  set_short(B_GREEN_F"j"GREY_F"ad"B_GREEN_F"e a"GREY_F"dorne"+
            B_GREEN_F"d e"GREY_F"bon"
            B_GREEN_F"y v"GREY_F"ambrace"+B_GREEN_F"s"NORM);
  set_long(BLUE_F"\
  This item is made of a metal alloy found only in meteors.\n");
  set_type("sleeves");
  set_ego(100);
  set_ac(11); 
  set_bonus_str(1);
  set_bonus_sta(1);
  set_weight(10); 
  set_value(1500); 
}


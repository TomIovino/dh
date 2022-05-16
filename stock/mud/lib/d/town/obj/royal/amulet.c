#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("necklace");
  set_short(B_CYAN_F"m"GREY_F"ithri"B_CYAN_F"l p"GREY_F"raye"+
       B_CYAN_F"r b"GREY_F"ea"
       B_CYAN_F"d n"GREY_F"ecklac"+B_CYAN_F"e"NORM);
  set_long(BLUE_F"\
  This item is made of a metal alloy found only in meteors.\n");
  set_type("amulet");
  set_ego(100);
  set_ac(11);
  set_bonus_dex(1);
  set_bonus_str(1);
  set_weight(10); 
  set_value(1500); 
}


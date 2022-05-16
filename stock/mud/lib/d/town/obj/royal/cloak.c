#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("cape");
  set_short(B_RED_F"r"GREY_F"ub"B_RED_F"y a"GREY_F"dorne"+
       NORM+B_RED_F"d f"GREY_F"orma"
       NORM+B_RED_F"l c"GREY_F"ap"+B_RED_F"e"NORM);
  set_long(BLUE_F"\
  This item is made of a metal alloy found only in meteors.\n");
  set_type("cloak");
  set_ego(100);
  set_ac(11);
  set_bonus_dex(1);
  set_bonus_agi(1);
  set_weight(10); 
  set_value(1500); 
}


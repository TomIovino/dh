#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("gauntlets");
  set_short(B_WHITE_F"s"GREY_F"ilve"B_WHITE_F"r e"GREY_F"tche"+
       NORM+B_WHITE_F"d e"GREY_F"bon"
       NORM+B_WHITE_F"y g"GREY_F"auntlet"+B_WHITE_F"s"NORM);
  set_long(BLUE_F"\
  This item is made of a metal alloy found only in meteors.\n");
  set_type("gloves");
  set_ego(100);
  set_ac(11);
  set_bonus_foc(1);
  set_bonus_agi(1);
  set_weight(10); 
  set_value(1500); 
}


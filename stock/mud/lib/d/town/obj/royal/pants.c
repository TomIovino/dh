#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("greaves");
  set_short(B_RED_F"c"GREY_F"rysta"B_RED_F"l a"GREY_F"dorne"+
       NORM+B_RED_F"d e"GREY_F"bon"
       NORM+B_RED_F"y g"GREY_F"reave"+B_RED_F"s"NORM);
  set_long(BLUE_F"\
  This item is made of a metal alloy found only in meteors.\n");
  set_type("pants");
  set_ego(100);
  set_ac(11);
  set_bonus_sta(1);
  set_bonus_foc(1);
  set_weight(10); 
  set_value(1500); 
}


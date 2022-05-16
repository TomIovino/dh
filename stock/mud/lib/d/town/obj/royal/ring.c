#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("ring");
  set_short(B_GREEN_F"e"GREY_F"meral"B_GREEN_F"d s"GREY_F"tudde"+
            B_GREEN_F"d e"GREY_F"bon"
            B_GREEN_F"y r"GREY_F"in"+B_GREEN_F"g"NORM);
  set_long(BLUE_F"\
  This item is made of a metal alloy found only in meteors.\n");
  set_type("ring");
  set_ego(100);
  set_ac(11);
  set_bonus_wis(1);
  set_bonus_int(1);
  set_weight(10); 
  set_value(1500); 
}


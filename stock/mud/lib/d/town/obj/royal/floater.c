#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("sphere");
  set_short(B_MAGENTA_F"a"GREY_F"damantiu"B_MAGENTA_F"m s"GREY_F"wirle"+
       NORM+B_MAGENTA_F"d e"GREY_F"bon"
       NORM+B_MAGENTA_F"y s"GREY_F"pher"+B_MAGENTA_F"e"NORM);
  set_long(BLUE_F"\
  This item is made of a metal alloy found only in meteors.\n");
  set_type("floating");
  set_ego(100);
  set_ac(11);
  set_bonus_dex(1);
  set_bonus_wis(1);
  set_weight(10); 
  set_value(1500); 
}


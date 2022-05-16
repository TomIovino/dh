#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("wrist");
  set_alias("wristbands");
  set_short(B_MAGENTA_F"v"GREY_F"elve"B_MAGENTA_F"t l"GREY_F"ace"+
       NORM+B_MAGENTA_F"d e"GREY_F"bon"
       NORM+B_MAGENTA_F"y w"GREY_F"ristband"+B_MAGENTA_F"s"NORM);
  set_long(BLUE_F"\
  This item is made of a metal alloy found only in meteors.\n");
  set_type("wrists");
  set_ego(100);
  set_ac(11);
  set_bonus_agi(1);
  set_bonus_int(1);
  set_weight(10); 
  set_value(1500); 
}


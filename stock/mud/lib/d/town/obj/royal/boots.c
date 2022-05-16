#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("boots");
  set_short(B_BLUE_F"s" GREY_F"apphir"B_BLUE_F"e s"GREY_F"tudde"+
            B_BLUE_F"d e"GREY_F"bon"
            B_BLUE_F"y b"GREY_F"oot"+B_BLUE_F"s"NORM);
  set_long(BLUE_F"\
  This item is made of a metal alloy found only in meteors.\n");
  set_type("boots");
  set_ego(100);
  set_ac(11);
  set_bonus_str(1);
  set_bonus_foc(1);
  set_weight(10); 
  set_value(1500); 
}


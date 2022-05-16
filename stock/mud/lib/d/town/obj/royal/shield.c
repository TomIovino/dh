#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("buckler");
  set_short(B_BLUE_F"m"GREY_F"idnightge"B_BLUE_F"m s"GREY_F"tudde"+
            B_BLUE_F"d e"GREY_F"bon"
            B_BLUE_F"y b"GREY_F"uckle"+B_BLUE_F"r"NORM);
  set_long(BLUE_F"\
  This items is made of an metal alloy found only in meteriors.\n");
  set_type("shield");
  set_ego(100);
  set_ac(17);
  set_bonus_pri(3);
  set_weight(17); 
  set_value(3000); 
}


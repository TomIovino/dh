#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("belt");
  set_short(YELLOW_F"a"GREY_F"mbe"YELLOW_F"r s"GREY_F"tudde"+
       NORM+YELLOW_F"d l"GREY_F"eathe"
       NORM+YELLOW_F"r b"GREY_F"el"+YELLOW_F"t"NORM);
  set_long(BLUE_F"\
  This item is made of a metal alloy found only in meteors.\n");
  set_type("belt");
  set_ego(100);
  set_ac(11);
  set_bonus_wis(1);
  set_bonus_foc(1);
  set_weight(10); 
  set_value(1500); 
}


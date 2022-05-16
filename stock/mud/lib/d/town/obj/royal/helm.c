#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("helm");
  set_short(YELLOW_F"g"GREY_F"ol"B_YELLOW_F"d e"GREY_F"tche"+
       NORM+YELLOW_F"d e"GREY_F"bon"
       NORM+YELLOW_F"y h"GREY_F"el"+B_YELLOW_F"m"NORM);
  set_long(BLUE_F"\
  This item is made of a metal alloy found only in meteors.\n");
  set_type("helm");
  set_ego(100);
  set_ac(11);
  set_bonus_sta(1);
  set_bonus_str(1);
  set_weight(10); 
  set_value(1500); 
}


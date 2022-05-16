#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("plate");
  set_short(B_WHITE_F"p"GREY_F"ear"B_WHITE_F"l s"GREY_F"tudde"+
       B_WHITE_F"d e"GREY_F"bon"+
       B_WHITE_F"y p"GREY_F"lat"+B_WHITE_F"e"NORM);
  set_long(BLUE_F"\
  This item is made of a metal alloy found only in meteors.\n");
  set_type("armour");
  set_ego(100);
  set_ac(25);
  set_weight(25); 
  set_value(3000); 
}


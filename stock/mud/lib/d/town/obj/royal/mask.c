#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("mask");
  set_short(B_WHITE_F"s"GREY_F"ilve"B_WHITE_F"r e"GREY_F"tche"+
       B_WHITE_F"d m"GREY_F"as"B_WHITE_F"k");
  set_long(BLUE_F"\
  This item is made of a metal alloy found only in meteors.\n");
  set_type("mask");
  set_ego(100);
  set_ac(11);
  set_bonus_dex(1);
  set_bonus_agi(1);
  set_weight(10); 
  set_value(1500); 
}


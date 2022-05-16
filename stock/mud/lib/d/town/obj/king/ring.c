#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("ring");
  set_short(B_CYAN_F"shiny skyblue ring"NORM);
  set_long(B_CYAN_F"\
This item is made of an unknown reflective material.\n");
  set_type("ring");
  set_ego(50);
  set_ac(12);            // item with bonuses are level 90 on 150 npc
  set_dpb(1);            // its^C skyblue =)^*
  set_apb(1);            // its^B reflective ;)^*
  set_weight(15);        // keeping it heavy not for newbies
  set_value(10000);      // entice them to sell this item =)
}


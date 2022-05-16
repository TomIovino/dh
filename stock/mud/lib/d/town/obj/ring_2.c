#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("ring");
  set_short(B_WHITE_F"silver ring"+NORM);
  set_long("A ring forged from silver.\n");
  set_type("ring");
  set_ac(5);
  set_weight(6); 
  set_value(250);
}


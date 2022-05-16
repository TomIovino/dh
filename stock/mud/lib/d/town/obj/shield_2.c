#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("shield");
  set_short(B_WHITE_F"silver shield"+NORM);
  set_long("A shield forged from silver.\n");
  set_type("shield");
  set_ac(8);
  set_weight(10); 
  set_value(500);
}


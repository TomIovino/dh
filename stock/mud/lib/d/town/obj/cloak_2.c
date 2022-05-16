#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("cloak");
  set_short(B_WHITE_F"silver chainmail cloak"+NORM);
  set_long("A chainmail cloak forged from silver.\n"+NORM);
  set_type("cloak");
  set_ac(5);
  set_weight(5); 
  set_value(2000);
}


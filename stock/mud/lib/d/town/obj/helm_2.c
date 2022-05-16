#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("helm");
  set_short(B_WHITE_F"silver chainmail helm"+NORM);
  set_long("A chainmail helm forged from silver.\n");
  set_type("helm");
  set_ac(5);
  set_weight(5); 
  set_value(2000);
}


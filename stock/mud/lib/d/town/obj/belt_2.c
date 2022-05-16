
#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("belt");
  set_short(B_WHITE_F"silver chainmail belt"+NORM);
  set_long("A chainmail belt forged from silver.\n");
  set_type("belt");
  set_ac(4);
  set_weight(4); 
  set_value(1000);
}


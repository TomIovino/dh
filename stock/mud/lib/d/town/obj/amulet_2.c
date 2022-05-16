#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("neckguard");
  set_short(B_WHITE_F"silver chainmail neckguard"+NORM);
  set_long("A neckguard forged from silver.\n");
  set_type("amulet");
  set_ac(5);
  set_weight(5); 
  set_value(2000);
}


#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("ankleband");
  set_alias("anklebands");
  set_short(BROWN_F+"soft leather ankleband"+NORM);
  set_long(BROWN_F+
"Simple anklebands made of sewn pieces of leather.\n");
  set_type("ankles");
  set_ac(4);
  set_weight(4); 
  set_value(1000);
}


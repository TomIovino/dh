#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("wristband");
  set_alias("wristbands");
  set_short(BROWN_F+"soft leather wristband"+NORM);
  set_long(BROWN_F+
"Simple wristbands made of sewn pieces of leather.\n");
  set_type("wrists");
  set_ac(2);
  set_weight(3); 
  set_value(10);
}


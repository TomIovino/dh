#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("gloves");
  set_short(BROWN_F+"soft leather gloves"+NORM);
  set_long(BROWN_F+
"Simple gloves made of sewn pieces of leather.\n");
  set_type("gloves");
  set_ac(4);
  set_weight(4); 
  set_value(1000);
}


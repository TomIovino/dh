#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("cap");
  set_short(BROWN_F+"soft leather cap"+NORM); 
  set_long(BROWN_F+
  "A cap made of leather which protects the user from some harm.\n");
  set_type("helm");
  set_ac(4);
  set_weight(4); 
  set_value(1000);
}


#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("marble");
  set_short(BROWN_F"swirling marble"+NORM);
  set_long(WHITE_F+
"A black and white colored marble. Looking at it is hard because\n"+
"it sheds a magic light and the colors seem to be constantly swirling.\n");
  set_type("floating");
  set_light(1);
  set_ac(4);
  set_weight(2); 
  set_value(10);
}


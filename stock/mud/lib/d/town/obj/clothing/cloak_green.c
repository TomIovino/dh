#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;
#define COLOR  "green"
#define ANSI GREEN_F

reset(arg) {
  if(arg) return;
  set_name("cloak");
  set_alias(COLOR+" cloak");
  set_short(ANSI+COLOR+" velour cloak"+NORM);
  set_long("An expensive "+ANSI+COLOR+" velour cloak."+NORM+"\n");
  set_type("cloak");
  set_ac(2);
  set_weight(2);
  set_value(5000);
}


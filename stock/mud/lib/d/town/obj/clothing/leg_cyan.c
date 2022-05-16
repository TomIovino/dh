#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;
#define COLOR  "cyan"
#define ANSI CYAN_F

reset(arg) {
  if(arg) return;
  set_name("leggings");
  set_alias(COLOR+" leggings");
  set_short(ANSI+COLOR+" velour leggings"+NORM);
  set_long("An expensive pair of "+ANSI+COLOR+" velour leggings."+NORM+"\n");
  set_type("pants");
  set_ac(2);
  set_weight(2);
  set_value(5000);
}


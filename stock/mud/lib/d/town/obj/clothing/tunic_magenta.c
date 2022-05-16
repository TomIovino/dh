#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;
#define COLOR  "magenta"
#define ANSI MAGENTA_F

reset(arg) {
  if(arg) return;
  set_name("tunic");
  set_alias(COLOR+" tunic");
  set_short(ANSI+COLOR+" velour tunic"+NORM);
  set_long("An expensive "+ANSI+COLOR+" velour tunic."+NORM+"\n");
  set_type("armour");
  set_ac(2);
  set_weight(2);
  set_value(5000);
}


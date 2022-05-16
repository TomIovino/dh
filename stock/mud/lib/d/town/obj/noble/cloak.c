#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("cloak");
  set_alias("chaincloak");
  set_short(B_WHITE_F+"ornate silver cloak "YELLOW_F
          "with a gold crest"+NORM);
  set_long(WHITE_F+
     "A beautifully ornate cloak crafted of mesh silver and gold.\n"+
  NORM);
  set_type("cloak");
  set_ac(6);
  set_bonus_int(1);
  set_weight(6); 
  set_value(500);
}


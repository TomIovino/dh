#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("platemail");
  set_alias("plate");
  set_short(B_WHITE_F+"ornate silver platemail "YELLOW_F
            "with a gold crest"+NORM);
  set_long(WHITE_F+
     "A beautifully ornate platemail crafted of silver and gold.\n"+
  NORM);
  set_type("armour");
  set_ac(14);
  set_bonus_str(1);
  set_weight(14); 
  set_value(2000);
}


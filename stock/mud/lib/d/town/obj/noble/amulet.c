#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("chain");
  set_alias("pendant");
  set_short(B_WHITE_F+"ornate silver chain "YELLOW_F
                      "with a gold pendant"+NORM);
  set_long(WHITE_F+
     "A beautifully ornate pendant crafted of silver and gold.\n"+
  NORM);
  set_type("amulet");
  set_ac(6);
  set_bonus_dex(1);
  set_weight(6); 
  set_value(500);
}


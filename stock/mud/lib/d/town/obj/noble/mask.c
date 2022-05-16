#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("mask");
  set_short(B_WHITE_F+"ornate silver mask "YELLOW_F
          "with a gold design"+NORM);
  set_long(WHITE_F+
     "A beautifully ornate mask crafted of silver and gold.\n"+
  NORM);
  set_type("mask");
  set_ac(6);
  set_bonus_wis(1);
  set_weight(6); 
  set_value(500);
}


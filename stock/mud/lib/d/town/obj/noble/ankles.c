#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("ankleguards");
  set_short(B_WHITE_F+"ornate silver ankleguards "YELLOW_F
          "with a gold design"+NORM);
  set_long(WHITE_F+
     "A beautifully ornate set of ankleguards crafted of silver and gold.\n"+
  NORM);
  set_type("ankles");
  set_ac(6);
  set_bonus_agi(1);
  set_weight(6); 
  set_value(500);
}


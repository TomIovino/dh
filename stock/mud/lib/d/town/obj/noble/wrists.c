#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("wristguards");
  set_short(B_WHITE_F+"ornate silver wristguards "YELLOW_F
          "with a gold design"+NORM);
  set_long(WHITE_F+
     "A beautifully ornate set of wristguards crafted of silver and gold.\n"+
  NORM);
  set_type("wrists");
  set_ac(6);
  set_bonus_int(1);
  set_weight(6); 
  set_value(500);
}

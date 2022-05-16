#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("towershield");
  set_short(B_WHITE_F+"ornate silver towershield "YELLOW_F
            "with a gold crest"+NORM);
  set_long(WHITE_F+
     "A beautifully ornate tower shield crafted of silver and gold.\n"+
  NORM);
  set_type("shield");
  set_ac(15);
  set_bonus_pri(1);
  set_weight(15); 
  set_value(2000);
}


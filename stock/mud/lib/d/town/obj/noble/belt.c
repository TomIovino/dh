#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("belt");
  set_alias("buckle");
  set_short(B_WHITE_F+"ornate silver belt "YELLOW_F
          "with a gold buckle"+NORM);
  set_long(WHITE_F+
     "A beautifully ornate belt crafted of silver and gold.\n"+
  NORM);
  set_type("belt");
  set_ac(6);
  set_bonus_sta(1);
  set_weight(6); 
  set_value(500);
}


#include <ansi.h>

inherit "inherit/armour";
reset(arg) {
  if(arg) return;
  set_name("cloak");
  set_alias("robes");
  set_short(GREY_F+"Death's Robes"+NORM);
  set_long(
      "  Hmmmmmmmmmm \n");
  set_type("cloak");
  set_ac(3);
  set_weight(4);
  set_sell_destruct(1);
  set_value(260);
  set_no_steal_flag(1);
}




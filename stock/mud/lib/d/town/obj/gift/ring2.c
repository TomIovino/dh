#include <mudlib.h>
#include <ansi.h>
inherit ARMOUR;
reset (arg) {
    if (arg) return;
    set_name("ring");
    set_alt_name("claddagh");
    set_short(YELLOW_F+"elegant golden claddagh ring"+NORM);
    set_long(
      "An elegantly fashioned golden claddagh ring, meant to be worn as a\n"+
      "sign of love and friendship.\n");
    set_type("ring");
    set_ac(1);
    set_weight(1);
    set_value(5000);
}
query_value() { return 5000; }

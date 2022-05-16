#include <mudlib.h>
#include <ansi.h>
inherit ARMOUR;
reset (arg) {
    if (arg) return;
    set_name("ring");
    set_alt_name("engagement");
    set_short(YELLOW_F+"golden "+B_WHITE_F+"diamond set"+YELLOW_F+" engagement ring"+NORM);
    set_long(
      "A golden engagement ring set with a beautiful blue-white diamond.\n");
    set_type("ring");
    set_ac(1);
    set_weight(1);
    set_value(1000);
}
query_value() { return 1000; }

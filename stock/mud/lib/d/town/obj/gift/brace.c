#include <mudlib.h>
#include <ansi.h>
inherit ARMOUR;
reset (arg) {
    if (arg) return;
    set_name("bracelet");
    set_short(B_WHITE_F+"silver chain-link bracelet"+NORM);
    set_long(
      "A silver chain-link bracelet given to you by a friend, to be worn\n"+
      "as a sign of your friendship.\n");
    set_type("wrists");
    set_ac(1);
    set_weight(1);
    set_value(750);
}
query_value() { return 750; }

#include <mudlib.h>
#include <ansi.h>
inherit TREASURE;
reset(arg) {
    if (arg) return;
    set_name("roses");
    set_alt_name("bouquet");
    set_alias("rose");
    set_short(GREEN_F+"bouquet of beautiful long-stemmed "+RED_F+"red roses"+NORM);
    set_long(
      "A bouquet of a dozen beautiful red roses with long green stems.\n");
    set_weight(1);
    set_value(600);
}

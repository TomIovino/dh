#include <mudlib.h>
#include <ansi.h>
inherit TREASURE;
reset(arg) {
    if (arg) return;
    set_name("nothing");
    set_short(GREY_F+"nothing"+NORM);
    set_long(
      GREY_F+"It is nothing.\n"+NORM);
    set_weight(0);
    set_value(0);
    set_info(GREY_F+"Absolutely nothing.\n"+NORM);
}

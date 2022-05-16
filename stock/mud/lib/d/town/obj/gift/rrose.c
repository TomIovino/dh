#include <mudlib.h>
#include <ansi.h>
inherit TREASURE;
reset(arg) {
    if (arg) return;
    set_name("rose");
    set_short(GREEN_F+"beautiful long-stemmed "+RED_F+"red rose"+NORM);
    set_long(
      "A beautiful red rose with a long stem.\n");
    set_weight(1);
    set_value(250);
}

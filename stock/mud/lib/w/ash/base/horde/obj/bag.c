
#include "obj.h"
#include <mudlib.h>
#include <ansi.h>
inherit CONTAINER;

void reset(status arg) {
    if(arg) return;
    set_name("bag");
    set_short(COLOR+"silk bag"+NORM);
    set_long(
    COLOR+"A bag made of silk.\n"+NORM);
    set_max_weight(50);
    set_value(700);
    set_weight(9);
}

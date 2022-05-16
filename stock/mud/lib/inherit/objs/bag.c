/* a bag */

#include <mudlib.h>
#include <ansi.h>
inherit CONTAINER;

void reset(status arg) {
    if(arg) return;
    set_name("bag");
    set_short(BROWN_F+"brown burlap bag"+NORM);
    set_long("\
The bag is made of burlap and although it is not too large, it does\n\
look strong.\n");
    set_max_weight(12);
    set_value(48);
    set_weight(3);
}

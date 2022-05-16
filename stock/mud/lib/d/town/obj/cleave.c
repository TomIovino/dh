#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

void reset(status arg){
    if(arg) return;

    set_name("axe");
    set_short(BROWN_F+"copper axe"+NORM);
    set_alias("axe");
    set_long(BROWN_F+
"A cheaply made axe created from copper.\n");
    set_wc(10);
    set_type("cleave");
    set_length(25);  
    set_weight(10);
    set_value(50);
}


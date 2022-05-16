#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

void reset(status arg){
    if(arg) return;

    set_name("quarterstaff");
    set_short(BROWN_F+"copper quarterstaff"+NORM);
    set_long(BROWN_F+
"A cheaply made quarterstaff created from copper.\n");
    set_wc(10);
    set_type("crush");
    set_length(25);  
    set_weight(10);
    set_value(1000);
}


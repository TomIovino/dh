#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

void reset(status arg){
    if(arg) return;

    set_name("shortsword");
    set_short(BROWN_F+"copper shortsword"+NORM);
    set_alias("weapon");
    set_long(BROWN_F+
"A cheaply made shortsword created from copper.\n");
    set_wc(6);
    set_type("slash");
    set_length(15);  
    set_weight(8);
    set_value(50);
}


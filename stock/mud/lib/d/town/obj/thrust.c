#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

void reset(status arg){
    if(arg) return;

    set_name("dagger");
    set_short(BROWN_F+"copper dagger"+NORM);
    set_alias("dagger");
    set_long(BROWN_F+
"A cheaply made dagger created from copper.\n");
    set_wc(6);
    set_type("thrust");
    set_length(15);  
    set_weight(8);
    set_value(50);
}


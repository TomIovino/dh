#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

void reset(status arg){
    if(arg) return;

    set_name("longsword");
    set_short(B_WHITE_F+"silver longsword"+NORM);
    set_alias("slash");
    set_long(B_WHITE_F+
    "A longsword made of silver.\n");
    set_wc(14);
    set_type("slash");
    set_length(35);  
    set_weight(14);
    set_value(1000);
}


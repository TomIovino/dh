#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

void reset(status arg){
    if(arg) return;
    set_name("spear");
    set_short(B_WHITE_F+"silver spear"+NORM);
    set_alias("spear");
    set_long(B_WHITE_F+
    "A spear made of silver.\n");
    set_wc(14);
    set_type("thrust");
    set_length(35);  
    set_weight(14);
    set_value(1000);
}


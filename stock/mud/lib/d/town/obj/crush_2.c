#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

void reset(status arg){
    if(arg) return;

    set_name("staff");
    set_short(B_WHITE_F+"silver staff"+NORM);
    set_alias("crush");
    set_long(B_WHITE_F+
   "A staff made of silver.\n");
    set_wc(14);
    set_type("crush");
    set_length(35);  
    set_weight(14);
    set_value(1000);
}


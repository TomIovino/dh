#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

void reset(status arg){
    if(arg) return;

    set_name("polearm");
    set_short(B_WHITE_F+"silver polearm"+NORM);
    set_alias("cleave");
    set_long(B_WHITE_F+
  "A long pole with a silver blade attached to one end.\n");
    set_wc(14);
    set_type("cleave");
    set_length(35);  
    set_weight(14);
    set_value(2000);
}


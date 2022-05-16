#include <ansi.h>
#include <mudlib.h>
#include "path.h"

inherit ARMOUR;
 
reset (arg){
    if(arg) return;
    set_weight(2);
    set_ac(1);
    set_value(100);
    set_name("runestone");
    set_alias("rune");
    set_type("shield");
    set_short(B_WHITE_F"runestone"NORM);
    set_long("A smooth runestone of "KI".\n"+NORM);
    set_extra_info("A strange and ancient rune.\n");
    set_sell_destruct(1);
}

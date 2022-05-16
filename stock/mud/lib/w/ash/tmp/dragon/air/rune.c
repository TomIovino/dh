#include <ansi.h>
#include <mudlib.h>
#include "path.h"

inherit TREASURE;
 
reset (arg){
    if(arg) return;
    set_weight(2);
    set_value(100);
    set_name("runestone");
    set_alias("rune");
    set_short(ADV" runestone"NORM);
    set_long("A smooth "ADV" "KI" runestone.\n"+NORM);
    set_extra_info("A strange and ancient rune possibly used in an\n"+
                   "ancient magical spell or device of some sort.\n");
    set_sell_destruct(1);
}

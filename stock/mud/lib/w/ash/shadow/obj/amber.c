#include <ansi.h>
#include <mudlib.h>

inherit TREASURE;
 
reset (arg){
    if(arg) return;
 
    set_weight(1);
    set_value(250);
    set_name("amber");
    set_alias("pentagon");
    set_short(YELLOW_F+"pentagonal amber"+NORM);
    set_long(NORM+
"The small gem is translucent and is cut nicely to be\n"+
"a perfect pentagon. You cant help but wonder if it\n"+
"was made to be 'inserted' into an object of some sort.\n"+
NORM); 
    set_sell_destruct(1);
}

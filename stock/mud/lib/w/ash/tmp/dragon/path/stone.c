#include <ansi.h>
#include <mudlib.h>

inherit TREASURE;
 
reset (arg){
    if(arg) return;
    set_weight(1);
    set_value(1);
    set_name("stone");
    set_short(GREY_F+"smooth stone"+NORM);
    set_long("A smooth stone made of grantite.\n");
    set_extra_info("It doesn't look very valuable.");
    set_sell_destruct(1);
}

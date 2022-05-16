 
#include <ansi.h>
#include <mudlib.h>
inherit TREASURE;
 
reset (arg){
    if(arg) return;
 
    set_weight(1);
    set_value(1);
    set_name("rock");
    set_short(BROWN_F+"smooth rock"+NORM);
    set_long("A small smooth grey rock.\n");
    set_extra_info("It doesn't look very valuable.");
}

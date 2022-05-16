#include <ansi.h>
#include <mudlib.h>

inherit TREASURE;
 
reset (arg){
    if(arg) return;
 
    set_weight(1);
    set_value(101);
    set_name("gem");
    set_short(RED_F+"small gem"+NORM);
    set_long("\
The small red gem is translucent and is cut nicely. It doesn't look \n\
that valuable, but it is neat to look at.\n");
    set_extra_info("It doesn't look very valuable.");
    set_sell_destruct(1);
}

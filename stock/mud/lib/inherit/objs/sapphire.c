 
#include <ansi.h>
inherit "inherit/treasure";
 
reset (arg){
    if(arg) return;
 
    set_weight(1);
    set_value(875);
    set_name("sapphire");
    set_short(B_BLUE_F+"blue sapphire"+NORM);
    set_long("\
    The sapphire is shinny blue and semi precious.\n");
}

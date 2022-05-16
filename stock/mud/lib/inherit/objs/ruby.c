 
#include <ansi.h>
inherit "inherit/treasure";
 
reset (arg){
    if(arg) return;
 
    set_weight(1);
    set_value(800);
    set_name("ruby");
    set_short(RED_F+"red ruby"+NORM);
    set_long("\
    The ruby is shinny red and semi precious.\n");
}

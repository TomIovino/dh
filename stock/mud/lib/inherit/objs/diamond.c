 
#include <ansi.h>
inherit "inherit/treasure";
 
reset (arg){
    if(arg) return;
 
    set_weight(1);
    set_value(1000);
    set_name("diamond");
    set_short(B_WHITE_F+"white diamond"+NORM);
    set_long("\
    The diamond is shinny white and semi precious.\n");
}

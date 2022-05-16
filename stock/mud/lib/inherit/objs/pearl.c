 
#include <ansi.h>
inherit "inherit/treasure";
 
reset (arg){
    if(arg) return;
 
    set_weight(1);
    set_value(900);
    set_name("pearl");
    set_short(B_WHITE_F+"white pearl"+NORM);
    set_long("\
The pearl is shinny white, but doesn't look very valuable.\n");
    set_extra_info("It doesn't look very valuable.");
}

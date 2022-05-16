#include <ansi.h>
inherit "inherit/treasure";

reset (arg){
    if(arg) return;

    set_weight(1);
    set_value(123);
    set_name("perfume");
    set_short(B_RED_F+"small vial of perfume"+NORM);
    set_long("\
This small vial is filled with a red liquid which smells like perfume.\n");
    set_extra_info("It doesn't look very valuable.");
}


#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("cloak");
    set_alt_name("rune");
    set_short(B_RED_F"rune etched cloak"+NORM);
    set_long("This armour is made of the finest metals.\n"+
             "It also bears obvious magical enhancement.\n");
    set_type("cloak");
    set_ac(14);  
    set_weight(14);
    set_value(1000);
    set_ego(101);
}

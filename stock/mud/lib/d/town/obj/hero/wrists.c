
#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("bracers");
    set_alt_name("rune");
    set_short(B_RED_F"rune etched bracers"+NORM);
    set_long("This armour is made of the finest metals.\n"+
             "It also bears obvious magical enhancement.\n");
    set_type("wrists");
    set_ac(14);  
    set_weight(14);
    set_value(1000);
    set_ego(101);
}


#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("shield");
    set_alt_name("rune");
    set_short(B_RED_F"rune etched shield"+NORM);
    set_long("This armour is made of the finest metals.\n"+
             "It also bears obvious magical enhancement.\n");
    set_type("shield");
    set_ac(25);  
    set_weight(25);
    set_light(1);
    set_value(3200);
    set_ego(101);
}

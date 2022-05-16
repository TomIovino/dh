// for ivory

#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("shield");
    set_alt_name("shield");
    set_short(B_GREEN_F"pulsing emerald shield"NORM);
    set_long(B_GREEN_F"Pulsing emerald shield.\n"NORM);
    set_type("shield");
    set_ac(22);
    set_bonus_str(2);
    set_weight(21);
    set_value(3100);
    set_ego(70);
}

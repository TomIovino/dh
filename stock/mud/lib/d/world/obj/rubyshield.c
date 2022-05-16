// for ebony

#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("shield");
    set_alt_name("shield");
    set_short(B_RED_F"pulsing ruby shield"NORM);
    set_long(B_RED_F"Pulsing ruby shield.\n"NORM);
    set_type("shield");
    set_ac(22);
    set_bonus_dex(2);
    set_weight(21);
    set_value(3100);
    set_ego(70);
}

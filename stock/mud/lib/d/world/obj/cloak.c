// for siyah

#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("cloak");
    set_alt_name("cape");
    set_short(GREY_F"flowing grey cape"NORM);
    set_long(GREY_F"Flowing grey cape made of soft and warm wool.\n"NORM);
    set_type("cloak");
    set_ac(11);
    set_bonus_dex(3);
    set_weight(10);
    set_value(850);
    set_ego(70);
}

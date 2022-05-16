
#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("robe");
    set_alt_name("robes");
    set_short(B_WHITE_F"long flowing white robes"NORM);
    set_long(B_WHITE_F"Yes, long flowing white robes.\n"NORM);
    set_type("cloak");
    set_ac(12);
    set_bonus_wis(3);
    set_weight(10);
    set_value(3100);
    set_ego(70);
}

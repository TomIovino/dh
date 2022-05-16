
#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("robe");
    set_alt_name("robes");
    set_short(RED_F"long flowing red robes"NORM);
    set_long(RED_F"Yes, long flowing red robes.\n"NORM);
    set_type("armour");
    set_ac(19);
    set_bonus_foc(4);
    set_weight(15);
    set_value(3100);
    set_ego(100);
}

// for ivory

#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("ivorychain");
    set_alt_name("chainmail");
    set_short(B_WHITE_F"intricate ivory chainmail"NORM);
    set_long(B_WHITE_F"Intricate ivory chainmail.\n"NORM);
    set_type("armour");
    set_ac(22);
    set_bonus_wis(2);
    set_weight(21);
    set_value(3100);
    set_ego(70);
}

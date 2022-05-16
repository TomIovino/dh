// for ebony

#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("ebonychain");
    set_alt_name("chainmail");
    set_short(GREY_F"intricate ebony chainmail"NORM);
    set_long(GREY_F"Intricate ebony chainmail.\n"NORM);
    set_type("armour");
    set_ac(22);
    set_bonus_int(2);
    set_weight(21);
    set_value(3100);
    set_ego(70);
}

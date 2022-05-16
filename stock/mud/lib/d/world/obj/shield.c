// for robin

#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("shield");
    set_alt_name("fullshield");
    set_short(B_WHITE_F"solid white steel fullshield"NORM);
    set_long(B_WHITE_F"Solid white steel fullshield.\n"NORM);
    set_type("shield");
    set_ac(19);
    set_bonus_str(4);
    set_weight(20);
    set_value(3100);
    set_ego(70);
}

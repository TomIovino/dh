// for robin

#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("plate");
    set_alt_name("fullplate");
    set_short(B_RED_F"ornate red steel fullplate"NORM);
    set_long(B_RED_F"Ornate red steel fullplate.\n"NORM);
    set_type("armour");
    set_ac(20);
    set_bonus_sta(4);
    set_weight(21);
    set_value(3100);
    set_ego(70);
}

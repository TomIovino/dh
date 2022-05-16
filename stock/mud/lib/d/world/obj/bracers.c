// for threeteeth

#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("bracers");
    set_alt_name("grey");
    set_short(B_WHITE_F"shiny "GREY_F"grey metal bracers"NORM);
    set_long("These bracers are made of a shiny black metal.\n");
    set_type("wrists");
    set_ac(11);
    set_bonus_str(3);
    set_weight(10);
    set_value(850);
    set_ego(70);
}

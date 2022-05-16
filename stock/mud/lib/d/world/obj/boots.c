
#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("boots");
    set_alt_name("metallic");
    set_short(GREY_F+"shiny metallic boots"+NORM);
    set_long(GREY_F+"A shiny metallic boots.\n"+NORM);
    set_type("boots");
    set_ac(13);
    set_bonus_pri(4);
    set_weight(11);
    set_value(3100);
    set_ego(100);
}

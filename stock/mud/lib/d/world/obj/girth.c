
#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("girth");
    set_alt_name("metallic");
    set_short(GREY_F+"shiny metallic girth"+NORM);
    set_long(GREY_F+"A shiny metallic girth.\n"+NORM);
    set_type("belt");
    set_ac(13);
    set_bonus_wis(4);
    set_weight(11);
    set_value(3100);
    set_ego(100);
}


#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("gloves");
    set_alt_name("metallic");
    set_short(GREY_F+"shiny metallic gloves"+NORM);
    set_long(GREY_F+"A shiny metallic gloves.\n"+NORM);
    set_type("gloves");
    set_ac(13);
    set_bonus_int(4);
    set_weight(11);
    set_value(3100);
    set_ego(100);
}

#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;
#define TP this_player()
#define QN query_name()

reset(status arg) {
    if(arg) return;
    set_name("club");
    set_alt_name("club");
    set_alias("gnarled");
    set_short(BROWN_F+"gnarled oak club"+NORM);
    set_long("A gnarled oak club.\n");
    set_wc(24);
    set_type("pound");
    set_length(25);
    set_ego(70);
    set_weight(22);
    set_value(1500);
    set_light(1);
}

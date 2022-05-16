#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;
#define TP this_player()
#define QN query_name()

reset(status arg) {
    if(arg) return;

    set_name("ohmna");
    set_alt_name("sword");
    set_alias("ivory");
    set_short(BOLD+WHITE_F+"intricate ivory longsword"+NORM);
    set_long("\
The beautiful sword Ohmna, carved of pure ivory and blessed by Dawn herself.\n");
    set_wc(25);
    set_type("slash");
    set_length(30);
    set_ego(100);
    set_weight(20);
    set_value(1500);
    set_light(1);
}

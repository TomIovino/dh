#include <mudlib.h>
#include <ansi.h>
inherit WEAPON;

reset(arg) {
    ::reset(arg);
    if(arg) return;
    set_name("deiranaiye");
    set_alias("sword");
    set_alt_name("blade");
    set_short(MAGENTA_F+"rune covered "+BROWN_F+"witchwood "+CYAN_F+"blade"+NORM);
    set_long("\
This exquisite blade gleams darkly. Long tracings of runes cover the entire\n\
hilt and blade. The runes glow softly upon occasion. Almost as if the sword\n\
had a conciousness of its own. \n");
    set_type("slash");
    set_length(30);
    set_wc(25);
    set_ego(100);
    set_weight(20);
    set_value(10);
    set_no_steal_flag(1);
}

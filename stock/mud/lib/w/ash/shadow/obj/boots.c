
#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("boots");
    set_type("boots");
    set_short("soft leather boots encrusted with "+
    RED_F"g"YELLOW_F+"e"+B_WHITE_F"m"+B_BLUE_F+"s"+NORM);
    set_long("Made with the finest of gems.\n");
    set_ac(4);  
    set_weight(4);
    set_value(80);
    set_bonus_dex(1);
}

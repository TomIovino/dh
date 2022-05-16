
#include <ansi.h>
#include <mudlib.h>
inherit ARMOUR;

reset(arg) {
    if(arg) return;
    set_name("armor");
    set_type("armour");
    set_short("chain armor encrusted with "+
    RED_F"g"YELLOW_F+"e"+B_WHITE_F"m"+B_BLUE_F+"s"+NORM);
    set_long("Made with the finest of gems.\n");
    set_ac(10);  
    set_weight(10);
    set_value(500);
    set_bonus_agi(1);
}

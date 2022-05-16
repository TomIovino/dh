#include <ansi.h>
#include <mudlib.h>
#include "../element.h"
inherit WEAPON;
#define TP this_player()
#define QN query_name()

reset (status arg){
    if (arg) return;
    set_name("sword");
    set_alias("blade");
    set_short(COLOR+METAL+" "+TOTEM+" hilted sword blade"+NORM); 
    set_type("slash");
    set_length(12);
    set_wc(8);
    set_weight(7);
    set_value(50);
}


#include <ansi.h>
#include <mudlib.h>
#include "../element.h"
inherit WEAPON;
#define TP this_player()
#define QN query_name()

reset (status arg){
    if (arg) return;
    set_name("sword");
    set_alias("claymore");
    set_short(COLOR+METAL" claymore sword with "+GEM+" "+TOTEM+" hilt"+NORM); 
    set_type("slash");
    set_length(30);
    set_wc(20);
    set_weight(20);
    set_value(4000);
}


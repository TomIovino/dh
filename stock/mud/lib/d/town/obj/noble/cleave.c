#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

void reset(status arg){
    if(arg) return;

    set_name("broadsword");
    set_alias("sword");
    set_short(B_WHITE_F+"ornate silver broadsword "YELLOW_F
                      "with a gold design"+NORM);
    set_long(WHITE_F+
    "A beautifully ornate broadsword crafted of silver and gold alloy.\n"+
    NORM);
    set_wc(16);
    set_type("cleave");
    set_length(40);  
    set_weight(15);
    set_value(2000);
}


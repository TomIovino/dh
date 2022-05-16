#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

void reset(status arg){
    if(arg) return;

    set_name("shortsword");
    set_alias("sword");
    set_short(B_WHITE_F+"ornate silver shortsword "YELLOW_F
                      "with a gold design"+NORM);
    set_long(WHITE_F+
    "A beautifully ornate shortsword crafted of silver and gold alloy.\n"+
    NORM);
    set_wc(16);
    set_type("thrust");
    set_length(35);  
    set_weight(15);
    set_value(2000);
}


#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

void reset(status arg){
    if(arg) return;

    set_name("staff");
    set_alias("staff");
    set_short(B_WHITE_F+"ornate silver staff "YELLOW_F
                      "with a gold design"+NORM);
    set_long(WHITE_F+
    "A beautifully ornate staff crafted of silver and gold alloy.\n"+
    NORM);
    set_wc(16);
    set_type("pound");
    set_length(40);  
    set_weight(15);
    set_value(2000);
}


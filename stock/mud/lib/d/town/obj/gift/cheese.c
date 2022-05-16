#include <ansi.h>
#include <mudlib.h>
inherit FOOD;

reset(arg){
    if (arg) return;
    set_name("cheese");
    set_alt_name("lump");
    set_short(YELLOW_F+"big lump of whiner's cheese"+NORM);
    set_long("A big lump of cheese that goes very well with whine.\n");
    set_consumer_msg("Ahh... that went down really well with your whine.\n");
    set_room_msg("@@query_name:$this_player()$@@ eats a little cheese to go with the whine.\n");
    set_weight(1);
    set_value(500);
}
query_value() { return 500; }

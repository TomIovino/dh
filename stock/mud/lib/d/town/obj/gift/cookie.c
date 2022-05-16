#include <ansi.h>
#include <mudlib.h>
inherit FOOD;

reset(arg){
    if (arg) return;
    set_name("cookie");
    set_short(B_RED_F+"big "+B_BLUE_F+"colourful "+B_GREEN_F+"cookie"+NORM);
    set_long("Wow, you were given this very special cookie! You must\n"+
      "be very special!\n");
    set_consumer_msg("By the way, this was meant as sarcasm :P\n");
    set_room_msg("@@query_name:$this_player()$@@ eats a a big colourful cookie.\n");
    set_weight(1);
    set_value(500);
}
query_value() { return 500; }

#include <ansi.h>
#include <mudlib.h>
inherit DRINK;

reset(arg){
    if (arg) return;
    set_name("beer");
    set_alt_name("mug");
    set_short(BROWN_F+"foamy mug of imported beer"+NORM);
    set_long("A tall mug filled with imported beer with rivers of foam pouring down\n"+
      "the sides.\n");
    set_consumer_msg("You tilt back and guzzle the beer, that was good stuff!\n");
    set_room_msg("@@query_name:$this_player()$@@ tilts back and guzzles the beer, looking very satisfied.\n");
    set_weight(1);
    set_value(75);
}
query_value() { return 75; }

#include <ansi.h>
#include <mudlib.h>
inherit FOOD;

reset(arg){
    if (arg) return;
    set_name("chocolates");
    set_alt_name("box");
    set_short(B_MAGENTA_F+"box of delicious chocolates"+NORM);
    set_long("A box of delicious chocolates of a number of tempting varieties.\n");
    set_consumer_msg("You gobble down chocolates by the handful, leaving chocolate smears all\n"+
      "over your mouth and face.\n");
    set_room_msg("@@query_name:$this_player()$@@ gobbles down chocolates by the handful, leaving chocolate\n"+
      "smeared all over the mouth and face.\n");
    set_weight(1);
    set_value(75);
}
query_value() { return 75; }

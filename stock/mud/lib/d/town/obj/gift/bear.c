#include <mudlib.h>
#include <ansi.h>
inherit TREASURE;
reset(arg) {
    if (arg) return;
    set_name("bear");
    set_alt_name("teddy");
    set_short(BROWN_F+"huggable teddy bear"+NORM);
    set_long(
      "A large huggable teddy bear covered with soft brown fur.\n");
    set_weight(1);
    set_value(100);
}
init() {
    ::init();
    add_action("hug","hug");
}
hug(str) {
    if(!str) return 0;
    if(present(str)!=this_object()) return 0;
    write(CYAN_F+"You give the teddy bear a big warm hug.\n");
    say(CYAN_F+this_player()->query_name()+" gives the teddy bear a big warm hug.\n",this_player());
    return 1;
}

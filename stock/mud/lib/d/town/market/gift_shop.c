#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {

    ::reset(arg);
    add_object("zed","/d/town/npc/zed");
    if(arg) return;
    set_short("Gift Shop");
    set_exits(({
      "/d/town/market/ave5","north",
      }));
    set_long(RED_F+
"The walls of this shop are lined with various gift items.\n"+
"Type 'list' for all the items that are available to purchase.\n");
    set_light(5);
}

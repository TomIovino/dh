#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {

    ::reset(arg);
    add_object("perf","/d/town/npc/perf");
    if(arg) return;
    set_short("General Store");
    set_exits(({
      "/d/town/market/ave1","south",
      }));
    set_long(RED_F+
"The walls of this shop are lined with just about everything.\n"+
"Type 'list' for all the items that are availible to purchase.\n");
    set_light(5);
}

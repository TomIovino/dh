
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {

    ::reset(arg);
    add_object("deneri","/d/town/npc/deneri");
    if(arg) return;
    set_short("Clothing Store");
    set_exits(({
      "/d/town/market/ave5","south",
      }));
    set_long(RED_F+
"The walls of this shop are lined with various articles of clothing.\n"+
"Type 'list' for all the items that are available to purchase.\n");
    set_light(5);
}

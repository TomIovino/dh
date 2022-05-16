#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {

    ::reset(arg);
    add_object("stev","/d/town/npc/stev");
    if(arg) return;
    set_short("Armour Store");
    set_exits(({
      "/d/town/market/ave2","south",
      }));
    set_long(RED_F+
"The walls of this shop are lined with various kinds of armour.\n"+
"Type 'list' for all the items that are availible to purchase.\n");
    set_light(5);
}

#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {

   add_object("tuji","/d/town/npc/tuji");
    if(arg) return;
    set_short("Weapon Store");
    set_exits(({
      "/d/town/market/ave2","north",
      }));
    set_long(RED_F+
"The walls of this shop are lined with various kinds of weapons.\n"+
"Type 'list' for all the items that are availible to purchase.\n");
    set_light(5);
}

#include <ansi.h>
#include <mudlib.h>
#include <ansi.h>
inherit ROOM;

void reset(status arg) {

    ::reset(arg);
    add_object("sylv","/d/town/npc/sylv");
    if(arg) return;
    set_short("Magic Store");
    set_exits(({
      "/d/town/market/ave4","north",
      }));
   set_long(RED_F+
"The walls of this shop are lined with various arcane materials.\n"+
"Type 'list' for all the items that are available to purchase.\n");
    set_light(5);
}

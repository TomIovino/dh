#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {

    ::reset(arg);
    add_object("siyah","/d/town/npc/siyah");
    if(arg) return;
    set_short("Book Store");
    set_exits(({
      "/d/town/market/ave5","north",
      }));
    set_long(BROWN_F+
"The walls are lined with books and scrolls that are for sale.\n"+
"Type 'list' for a list of all the books you can purchase here.\n");
    set_light(5);
}



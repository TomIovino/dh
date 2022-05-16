#include <ansi.h>
#include <mudlib.h>
#include <ansi.h>
#include "../element.h"

inherit ROOM;

void reset(status arg) {

    ::reset(arg);
    add_object("shopkeeper",PATH+"npc/mshopkeep");
    if(arg) return;
    set_short(MSHOP);
    set_exits(({
    PATH+"road/w3","south",
      }));
   set_long(COLOR+
"The walls of this shop are lined with variours arcane materials.\n"+
"Type 'list' for all the items that are availible to purchase.\n");
    set_light(5);
}

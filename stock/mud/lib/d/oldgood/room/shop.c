#include <ansi.h>
#include <mudlib.h>
#include <ansi.h>
#include "../element.h"

inherit ROOM;

void reset(status arg) {

    ::reset(arg);
    add_object("shopkeeper",PATH+"npc/shopkeep");
    if(arg) return;
    set_short(SHOP);
    set_exits(({
      PATH+"road/n3","west",
      }));
   set_long(COLOR+
"The walls of this shop are lined with armours and weapons.\n"+
"Type 'list' for all the items that are availible to purchase.\n");
    set_light(5);
}

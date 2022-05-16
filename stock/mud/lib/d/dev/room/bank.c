#include <mudlib.h>
#include <ansi.h>
#include "../element.h"
inherit ROOM;


void reset(status arg) {
    object obj;
    ::reset(arg);
    if(arg) return;
    add_object("banker",PATH+"npc/banker");
    set_short(BANK);
    set_long(COLOR+
"This is the "+BANK+" Here you can deposit and withdraw\n"+
"money so that it is safe at a modest cost to the customer.\n"+
    NORM);
    set_light(2);
    set_items(({
      }));
    set_exits(({
        PATH+"road/s3","east",
      }));
   }


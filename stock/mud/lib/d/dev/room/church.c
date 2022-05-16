#include <mudlib.h>
#include <ansi.h>
#include "../element.h"
inherit CHURCH;


void reset(status arg) {
    object obj;
    ::reset(arg);
    if(arg) return;
    add_object("vortex","d/town/obj/vortex");
    set_no_steal(1);
    set_no_fight(1);
    set_short(TEMPLE);
    set_long(COLOR+
"This is the "+TEMPLE+" of "+capitalize(ELEMENT)+".\n"+
"North you may find items that will help you on your journey.\n"+
    NORM);
    set_light(2);
    set_items(({
      }));
    set_exits(({
        PATH+"road/e2","south",
        PATH+"room/donation","north",
      }));
}


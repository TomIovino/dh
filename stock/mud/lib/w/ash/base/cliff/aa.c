
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(B_WHITE_F+
"Standing on platform on the side of a mountain in the sunshine of\n"+
"a warm summers day. Wild flowers dot the landscape giving the air a\n"+
"wonderful intoxicating scent.\n"+NORM);

 
set_items(({
    "sky","The sky is clear and the wind blows from the south.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    "d/town/road/mainsqr","south",
              }));
}

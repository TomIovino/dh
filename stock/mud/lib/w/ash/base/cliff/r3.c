
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(B_WHITE_F+
"Huge white rocks are everywhere of all sizes. West leads down and\n"+
"safely toward the forest. East leads up into the cliffs that offer\n"+
"the only way up this side of the mountain.\n"+
NORM);
    set_items(({
    "mountains","High mountains rise to the northeast.\n",
    "pathway","A rugged path around the mountains.\n",
    "sky#u#up","The sky is clear and the wind blows from the south.\n",
    "forest","The large forest beyond the mountains.\n",
    "rock#rocks","Large rocks bleached white by the sun.\n",
    "ground#d#down","The ground is hard and rocky.\n",
    "cliffs#cliff#mountain#side",
    "A high mountain with rugged cliffs jutting out from the side.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"r2","west",
    PATH+"3a","east",
              }));
}

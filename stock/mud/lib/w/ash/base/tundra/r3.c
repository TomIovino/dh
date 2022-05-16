
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(CYAN_F+
"The ocean and mountains meet here leaving only a narrow pathway.\n"+
"The pathway climbs in elevation as you go northwest around the\n"+
"large mountain range to the northeast. West the dark blue ocean\n"+
"waters can be seen until the sky meets the horizon.\n"+
NORM);
    set_items(({
    "ocean#water#waters","Dark blue and cold looking.\n",
    "pathway","A rugged path around the mountains.\n",
    "mountains#mountain#range","High mountains rise to the northeast.\n",
    "sky#u#up","The sky is clear and cloudless.\n",
    "ground#d#down","The ground is hard and frozen.\n",
    "snow","The snow blows harshly from the north.\n",
    "horizon","The distant line where the sky meets the ocean.\n",
    "tundra","A frozen windswept wasteland.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"3c","north",
   PATH+"r2","south",
              }));
}

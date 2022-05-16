
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(YELLOW_F+
"The ocean and mountains meet here leaving only a narrow pathway.\n"+
"The pathway descends in elevation as you go southeast around the\n"+
"large mountain range to the northeast. West the greenblue ocean\n"+
"waters can be seen until the sky meets the horizon.\n"+
NORM);
    set_items(({
    "mountains#mountain#range","High mountains rise to the northeast.\n",
    "ocean#water#waters","Greenblue and warm looking.\n",
    "pathway","A rugged path around the mountains.\n",
    "sand","Brown, small, grainy...\n",
    "dunes#hills#hill#dune","Made of sand...\n",
    "sky#u#up","The sky is clear and the wind blows from the south.\n",
    "ground#d#down","The ground is dry and sandy.\n",
    "horizon","The distant line where the sky meets the ocean.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"r2","south",
    "/d/world/y/25","north",
              }));
}

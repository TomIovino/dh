
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(B_WHITE_F+
"A large elevated plateau high in the mountains. Rocks and dirt have\n"+
"been bleached white from the sun. North of here is a white walled\n"+
"keep which has been built into the mountainside.\n"+
NORM);
    set_items(({
    "mountains","High mountains rise to the northeast.\n",
    "pathway","A rugged path around the mountains.\n",
    "sky#u#up","The sky is clear and the wind blows from the south.\n",
    "plateau","A large flat table of land.\n",
    "ground#rocks#rock#dirt#d#down",
    "The rocks and dirt that cover the plateau are a gleaming white.\n",
    "keep#mountainside",
    "A stone fortress is built into the side of the mountain.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    "/w/ash/base/federation/enter","north",
    PATH+"2b","south",
    PATH+"1a","west",
    PATH+"1c","east",
              }));
}

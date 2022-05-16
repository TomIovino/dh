
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(YELLOW_F+
"Dust swirls about the sandy ground as one trudges on through the desert.\n"+
"North of here is a keep built into the side of a mountain.\n"+
   NORM);
    set_items(({
    "sand","Brown, small, grainy...\n",
    "dunes#hills#hill#dune","Made of sand...\n",
    "desert","A dry, barren stretch of sand.\n",
    "sky#u#up","The sky is clear and the wind blows from the south.\n",
    "ground#d#down","The ground is dry and sandy.\n",
    "keep#side#mountain",
    "A stone fortress is built into the side of a high mountain.\n",
    "dust","Extremely fine particles of sand.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    "/w/ash/base/horde/enter","north",
    PATH+"2b","south",
    PATH+"1a","west",
    PATH+"1c","east",
              }));
}

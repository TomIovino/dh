
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(YELLOW_F+
"In every direction sand dunes meet the horizon of the dry desert\n"+
"sky. Looking behind you a trail of footprints is the only mark on\n"+
"this harsh land of nothingness.\n"+NORM);
    set_items(({
    "sand","Brown, small, grainy...\n",
    "dunes#hills#hill#dune","Made of sand...\n",
    "sky#u#up","The sky is clear and the wind blows from the south.\n",
    "ground#d#down","The ground is dry and sandy.\n",
    "horizon","The distant line where the sky meets the desert.\n",
    "trail#footprints#footprint#mark","Faint impressions in the sand.\n",
    "land#desert","A dry, barren stretch of sand.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"r3","west",
    PATH+"2a","north",
    PATH+"3b","east",
              }));
}


#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(YELLOW_F+
"Dust swirls about the sandy ground as one trudges on through the desert.\n"+
"Surrounding you as far as the eye can see is a seemingly endless,\n"+
"unbroken expanse of sand.\n"+
   NORM);
    set_items(({
    "sand","Brown, small, grainy...\n",
    "dunes#hills#hill#dune","Made of sand...\n",
    "sky#u#up","The sky is clear and the wind blows from the south.\n",
    "ground#d#down","The ground is dry and sandy.\n",
    "land#desert","A dry, barren stretch of sand.\n",
    "dust","Extremely fine particles of sand.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"2b","north",
    PATH+"3a","west",
    PATH+"3c","east",
              }));
}

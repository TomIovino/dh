
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(YELLOW_F+
"Dust swirls about the sandy ground as one trudges on through the desert.\n"+
"The seemingly endless expanse of sand is broken by a rugged mountain that\n"+
"rises high into the desert sky.\n"+
   NORM);
    set_items(({
    "sand","Brown, small, grainy...\n",
    "mountain","A large mountain looms over the desert.\n",
    "dunes#hills#hill#dune","Made of sand...\n",
    "sky#u#up","The sky is clear and the wind blows from the south.\n",
    "ground#d#down","The ground is dry and sandy.\n",
    "dust","Extremely fine particles of sand.\n",
    "land#desert","A dry, barren stretch of sand.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"1a","north",
    PATH+"3a","south",
    PATH+"2b","east",
   "/w/ash/base/order/rm1","up",
              }));
}

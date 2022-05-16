
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(CYAN_F+
"Bitter wind howls across the frozen wastelands of this forgotten\n"+
"realm of the far north. A few rugged trees dot the desolate\n"+
"landscape, and tough brush pokes up here and there through the\n"+
"permafrost. A rugged mountain rises high into the cold, clear sky.\n"+
    NORM);
    set_items(({
    "mountain","A large mountain looms over the tundra.\n",
    "sky#u#up","The sky is clear and cloudless.\n",
    "permafrost#ground#d#down",
    "The ground is permanently frozen by the frigid cold.\n",
    "wastelands#realm#landscape#tundra","A frigid windswept wasteland.\n",
    "underbrush#grass#shrubs#brush#trees#tree",
    "Its hard to believe such a thing exists in a place like this.\n",
    "snow","The snow blows harshly from the north.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"1a","north",
    PATH+"3a","south",
    PATH+"2b","east",
     "/w/ash/base/life/rm1","up",
              }));
}

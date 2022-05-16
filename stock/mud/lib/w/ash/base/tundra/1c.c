
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
"permafrost.\n"+
    NORM);
    set_items(({
    "mountains#mountain","Mountains surround the tundra in all directions.\n",
    "sky#u#up","The sky is clear and cloudless.\n",
    "ground#d#down#permafrost",
    "The ground is permanently frozen by the frigid cold.\n",
    "underbrush#grass#shrubs#brush#trees#tree",
    "It's hard to believe such a thing exists in a place like this.\n",
    "snow","The snow blows harshly from the north\n",
    "wastelands#realm#landscape#tundra","A frigid windswept wasteland.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"2c","south",
    PATH+"1b","west",
              }));
}


#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(CYAN_F+
"An area of vast frozen tundra is cradled in the great mountains.\n"+
"Nothing grows on these desolate plains except small patches of grass.\n"+
"A keep is built into the mountainside to the north.\n"+
NORM);   
    set_items(({
    "mountains#mountain","Mountains surround the tundra in all directions.\n",
    "sky#u#up","The sky is clear and cloudless.\n",
    "dirt#grass#ground#d#down#patches",
    "A few small clumps of grass poke up through the frozen dirt.\n",
    "keep#mountainside",
    "A stone fortress is built into the side of the mountain to the north.\n",
    "tundra#plain#plains","A frigid windswept wasteland.\n",
    "snow","The snow blows harshly from the north.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"2b","south",
    PATH+"1a","west",
    PATH+"1c","east",
    "/w/ash/base/alliance/enter","north",
              }));
}

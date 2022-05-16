
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(CYAN_F+
"An area of vast frozen tundra is cradled in the great mountains.\n"+
"Nothing grows on these desolate plains except small patches of grass.\n"+
"One mountain stands higher than the rest, rising into the cold, clear sky.\n"+
NORM);   
    set_items(({
    "mountains","Mountains surround the tundra in all directions.\n",
    "mountain","A large mountain looms over the tundra.\n",
    "sky#u#up","The sky is clear and cloudless.\n",
    "dirt#grass#ground#d#down#patches",
    "A few small clumps of grass poke up through the frozen dirt.\n",
    "snow","The snow blows harshly from the north\n",
    "plain#tundra#wastes#plains","A frigid windswept wasteland.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"1c","north",
    PATH+"3c","south",
    PATH+"2b","west",
    "/w/ash/base/water/rm1","up",
              }));
}






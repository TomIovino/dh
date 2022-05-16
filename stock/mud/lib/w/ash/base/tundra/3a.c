
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(CYAN_F+
"An area of vast frozen tundra is cradled in the great mountains.\n"+
"Nothing grows on these desolate plains except small patches of grass.\n"+
NORM);   
    set_items(({
    "mountains#mountain","Mountains surround the tundra in all directions.\n",
    "sky#u#up","The sky is clear and cloudless.\n",
    "dirt#grass#patches#d#down#ground",
    "A few small clumps of grass poke up through the frozen ground.\n",
    "snow","The snow blows harshly from the north\n",
    "plains#plain#tundra","A frigid windswept wasteland.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"2a","north",
    PATH+"3b","east",
              }));
}

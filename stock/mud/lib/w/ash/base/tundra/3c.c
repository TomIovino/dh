
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(CYAN_F+
"The wind savagely whips across this icy plain from all sides.\n"+
"An endless expanse of frigid wastes and frozen carcasses\n"+
"sprawls to the ends of vision... Seemingly nothing can\n"+
"survive in these savage conditions.\n"+NORM);
    set_items(({
    "mountains#mountain","Mountains surround the tundra in all directions.\n",
    "sky#u#up","The sky is clear and cloudless.\n",
    "dirt#ground#d#down#patches#grass",
    "A few small clumps of grass poke up through the frozen dirt.\n",
    "plain#tundra#wastes#plains",
    "A frigid windswept wasteland.\n",
    "carcass#carcasses","The frozen remains of dead creatures.\n",
    "snow","The snow blows harshly from the north\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"2c","north",
    PATH+"3b","west",
    PATH+"r3","south",
              }));
}

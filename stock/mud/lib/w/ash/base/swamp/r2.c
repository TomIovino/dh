
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(BROWN_F+
"The ocean and swamps meet here leaving only a narrow pathway.\n"+
"The pathway slopes southwest towards a forested swamp bog.\n"+
"All around waters can be seen until the sky meets the horizon.\n"+
NORM);
    set_items(({
    "ocean#water#waters","Greenblue and warm looking.\n",
    "swamps#swamp#bog",
    "A marshy stretch of land with a variety of plantlife growing in the\n"+
    "fertile mud.\n",
    "pathway","A sloping path through the swamp.\n",
    "horizon","The distant line where the sky meets the ocean.\n",
    "ground#d#down","The ground is soggy underfoot.\n",
    "sky#u#up","The sky is clear and cloudless.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"r1","east",
    PATH+"r3","south",
              }));
}

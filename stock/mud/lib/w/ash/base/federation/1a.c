
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
//NW CORNER
set_long(GREY_F+
ADJECTIVE+" stone walls from the north and west come\n"+
"together to form the corner of this keep. A hallway runs both\n"+
"east and south of here decorated on the outside walls with\n"+
"large banners of "+COAT_OF_ARMS+"\n"+
"A niche in the stone wall catches your attention.\n"+NORM);
   set_items(({
    "niche","A niche in the stone possibly from a heavy sword blow.\n",
    "banners#banner", COAT_OF_ARMS+NORM+"\n",
    "walls#wall",WALLS,
    "up#u#ceiling", ADJECTIVE+" stone ceiling.\n",
    "down#d#floor", ADJECTIVE+" stone floor.\n",
    "hallway","A passage in the keep.\n",
    "keep","A small stone fortress.\n",
    "corner","Where the walls meet.\n",
   }));
    set_light(2);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"nw","up",
    PATH+"2a","south",
    PATH+"1b","east",
              }));
}

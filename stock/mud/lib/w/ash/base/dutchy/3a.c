
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
//SW CORNER
set_long(GREY_F+
ADJECTIVE+" stone walls from the south and west come\n"+
"together to form the corner of this keep. A hallway runs both\n"+
"east and north of here decorated on the outside walls with\n"+
"large banners of "+COAT_OF_ARMS+"\n"+
"A mark of some sort is in the wooden beam in the ceiling.\n"+NORM);
 
    set_items(({
    "walls#wall",WALLS,
    "banner#banners",COAT_OF_ARMS+NORM+"\n",
    "mark#beam","It looks like a common glyph for danger.\n",
    "up#u#ceiling", ADJECTIVE+" stone ceiling.\n",
    "hallway","A passage in the keep.\n",
    "down#d#floor", ADJECTIVE+" stone floor.\n",
    "keep","A small stone fortress.\n",
    "corner","Where the walls meet.\n",
    }));
    set_light(2);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"sw","up",
    PATH+"2a","north",
    PATH+"3b","east",
              }));
}

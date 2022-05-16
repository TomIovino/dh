
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
//SE CORNER
set_long(GREY_F+
ADJECTIVE+" stone walls from the south and east come\n"+
"together to form the corner of this keep. A hallway runs both\n"+
"west and north of here decorated on the outside walls with\n"+
"large banners of "+COAT_OF_ARMS+"\n"+
"A large stain of some sort is on the castle floor.\n"+NORM);
    set_items(({
    "walls#wall",WALLS,
     "banner#banners",COAT_OF_ARMS+NORM+"\n",
     "stain",B_RED_F+"That can only be a stain caused by blood.\n",
    "up#u#ceiling", ADJECTIVE+" stone ceiling.\n",
    "down#d#floor", ADJECTIVE+" stone floor.\n",
    "hallway","A passage in the keep.\n",
    "keep#castle","A small stone fortress.\n",
    "corner","Where the walls meet.\n",
    }));
    set_light(2);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"se","up",
    PATH+"2c","north",
    PATH+"3b","west",
              }));
}

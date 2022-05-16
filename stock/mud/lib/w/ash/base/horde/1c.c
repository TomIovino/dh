
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
//NE CORNER
set_long(GREY_F+
ADJECTIVE+" stone walls from the north and east come\n"+
"together to form the corner of this keep. A hallway runs both\n"+
"west and south of here decorated on the outside walls with\n"+
"large banners of "+COAT_OF_ARMS+"\n"+
"A wooden support beam looks singed by fire.\n"+NORM);
 
    set_items(({
    "walls#wall",WALLS,
    "beam#mark","Looks like a firelance hit it.\n",
    "banners#banner",COAT_OF_ARMS+NORM+"\n",
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
    PATH+"ne","up",
    PATH+"2c","south",
    PATH+"1b","west",
              }));
}

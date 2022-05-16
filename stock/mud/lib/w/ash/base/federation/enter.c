
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    add_object("gateguard 1",PATH+"gateguard");
    add_object("gateguard 2",PATH+"gateguard");
  if(arg) return; 
set_long(GREY_F+
ADJECTIVE+" stone walls form the entrance to this keep.\n"+
"The east and west walls are decorated with large banners of\n"+
COAT_OF_ARMS+"\n"+
NORM);
   set_items(({
    "niche","A niche in the stone possibly from a heavy sword blow.\n",
    "entrance","The entry to the keep.\n",
    "keep","A small stone fortress.\n",
    "banners#banner", COAT_OF_ARMS+NORM+"\n",
    "walls#wall",WALLS,
    "up#u#ceiling", ADJECTIVE+" stone ceiling.\n",
    "down#d#floor", ADJECTIVE+" stone floor.\n",
   }));
    set_light(2);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"3b","north",
    EXIT,"south",
              }));
}

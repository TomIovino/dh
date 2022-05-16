
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    if(arg) return;
    set_long(BROWN_F+
"A large swamp area with very few paths that can be taken around the\n"+
"huge pits of mud, earthen matter and some possible quicksand. Here,\n"+
"a rugged mountain rises from the muddy swamp. To the north is a small\n"+
"keep on the edge of the bog.\n"+NORM);
    set_items(({
    "swamp#bog",
    "A marshy stretch of land with a variety of plantlife growing in the\n"+
    "fertile mud.\n",
    "mountain","A large mountain looms over the swamp.\n",
    "ground#d#down","The ground is soggy underfoot.\n",
    "path#paths","A few trails wind through the swamp.\n",
    "pits#pit#mud#matter#quicksand",
    "The swamp contains dangerous areas of deep mud, tangled undergrowth\n"+
    "and sucking quicksand.\n",
    "sky#u#up","The sky is clear and cloudless.\n",
    "keep#edge","A stone fortress lies at the edge of the swamp.\n",
    }));
    set_light(1);
    set_short(SHORT);
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"3a","south",
    PATH+"1a","north",
    PATH+"2b","east",
    "/w/ash/base/death/rm1","up",
              }));
}

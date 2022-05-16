#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
    if(arg) return;
    set_short("Market Avenue");
    set_long(RED_F+
"A cobblestone avenue leads east of here further into the market\n"+
"district of the city. West leads back into the city proper.\n"+
"Towards the north is a armour store and south is a weapon shop.\n"+
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
     "Well worn cobblestones worked togather to form the road.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    }));
    set_exits(({
	"d/town/market/ave1","west",
	"d/town/market/a_shop","north",
	"d/town/market/w_shop","south",
        "d/town/market/ave3","east",
      }));
   set_light(5);
}

#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
         add_object("guard","d/town/npc/guard_w");
    if(arg) return;
    set_short("Market Avenue");
    set_long(RED_F+
"A cobblestone avenue leads east of here further into the market\n"+
"district of the city. West leads back into the city proper.\n"+
"The general store is north and the pub is south of here.\n"+
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
     "Well worn cobblestones worked togather to form the road.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    }));
    set_exits(({
	"d/town/road/s3","west",
	"d/town/market/ave2","east",
	"d/town/market/g_shop","north",
	"d/town/market/p_shop","south",
      }));
   set_light(5);
}

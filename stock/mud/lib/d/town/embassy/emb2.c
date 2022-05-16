#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
    if(arg) return;
    set_short("Market Avenue");
    set_long(MAGENTA_F+
"A stone avenue leads west of here further into the embassy\n"+
"district of the city. East leads back into the city proper.\n"+
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road#avenue", BROWN_F+
     "Well worn cobblestones worked together to form the road.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    }));
    set_exits(({
        "/d/town/embassy/emb1","east",
	"d/town/embassy/emb3","west",
      }));
   set_light(5);
}

// edited by Jada/10/98
#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
    if(arg) return;
    set_short("Market Avenue");
    set_long(RED_F+
"Close to the east is the Pleasant Inn and Resting Place.\n"+
"West leads towards other shops and also to the city proper.\n"+
"Towards the north is a restaurant and south is a magic shop.\n"+
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
 "Well worn cobblestones worked together to form the road.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    }));
    set_exits(({
	"d/town/market/ave3","west",
        "d/town/market/ave5","east",
	"d/town/market/f_shop","north",
	"d/town/market/m_shop","south",
      }));
   set_light(5);
}

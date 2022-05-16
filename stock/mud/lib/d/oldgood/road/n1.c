#define AWAY "north"
#define TOWARDS "south"
#define ROAD_NAME "Northern Road"
#include "../element.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
   if(arg) return;
    set_type(ELEMENT);
    set_short(ROAD_NAME);
    set_long(COLOR+
"Directly south is the the main square of the city of "+TOWN+".\n"+
"A ways north is a shop sign for "+SHOP+".\n"+
IN+"\n"+
    NORM);
    set_items(({
    "u#up#sky",COLOR+UP+".\n",
    "d#down#street#ground#road",COLOR+DOWN+".\n",
    "walls#gates#citywalls",COLOR+WALLS+".\n",
   }));
   set_smell(SMELL"\n");
   set_listen(SOUND"\n");
   set_light(5);
   set_exits(({
	  PATH+"road/mainsqr",TOWARDS,
          PATH+"road/n2",AWAY,
   }));

}



#define AWAY "east"
#define TOWARDS "west"
#define ROAD_NAME "Eastern Road"
#include "../element.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
   if(arg) return;
    set_type(ELEMENT);
    set_short(ROAD_NAME);
    set_long(COLOR+
"Directly west is the the main square of the city of "+TOWN+".\n"+
"A banner of the "+TEMPLE+" is directly east of here.\n"+
"A ways east is a sign for "+MEET+".\n"+
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
          PATH+"road/e2",AWAY,
   }));

}



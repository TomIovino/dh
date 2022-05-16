#define AWAY "west"
#define TOWARDS "east"
#define ROAD_NAME "Western Road"
#include "../element.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
   if(arg) return;
    set_type(ELEMENT);
    set_short(ROAD_NAME);
    set_long(COLOR+
"Directly east is the the main square of the city of "+TOWN+".\n"+
"Further west of here you can see a sign for "+MSHOP+".\n"+
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
          PATH+"road/w2",AWAY,
   }));

}



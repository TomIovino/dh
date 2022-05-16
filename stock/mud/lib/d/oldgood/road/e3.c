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
"West leads slightly uphill to the main square of "+TOWN+".\n"+
"Directly east are the protective "+TOWN+" town gates.\n"+
"South is the entrance for "+MEET+".\n"+
"A banner of the "+TEMPLE+" is west of here.\n"+
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
          PATH+"road/egates",AWAY,
	  PATH+"road/e2",TOWARDS,
          PATH+"room/board3","south",
   }));

}



#define AWAY "north"
#define TOWARDS "south"
#define ROAD_NAME "Northern Road"
#include "../element.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
    add_object("citizen",PATH+"npc/citizen");
   if(arg) return;
    set_type(ELEMENT);
    set_short(ROAD_NAME);
    set_long(COLOR+
"South leads slightly uphill to the main square of "+TOWN+".\n"+
"A sign for "+SHOP+" is north of here.\n"+  // SHOP IS LONG
"A ways north of here are the gates of the city of "+TOWN+".\n"+
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
	  PATH+"road/n1",TOWARDS,
          PATH+"road/n3",AWAY,
   }));

}



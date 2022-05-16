#define AWAY "south"
#define TOWARDS "north"
#define ROAD_NAME "Southern Road"
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
"North leads slightly uphill to the main square of "+TOWN+".\n"+
"The "+PUB+" entrance is directly east.\n"+  // SHOP IS LONG
"A sign for "+BANK+" is south of here.\n"+  // SHOP IS LONG
"A ways south of here are the gates of the city of "+TOWN+".\n"+
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
	  PATH+"road/s1",TOWARDS,
          PATH+"road/s3",AWAY,
          PATH+"room/pub","east",
   }));

}



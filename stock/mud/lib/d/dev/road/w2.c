#define AWAY "west"
#define TOWARDS "east"
#define ROAD_NAME "Western Road"
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
"East leads slightly uphill to the main square of "+TOWN+".\n"+
"The "+PROF_B+" entrance is directly south.\n"+  // SHOP IS LONG
"A sign for "+MSHOP+" is west of here.\n"+  // SHOP IS LONG
"A ways west of here are the gates of the city of "+TOWN+".\n"+
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
	  PATH+"road/w1",TOWARDS,
          PATH+"road/w3",AWAY,
          PATH+"room/prof_b1","south",
   }));

}



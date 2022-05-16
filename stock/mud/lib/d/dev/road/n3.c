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
"South leads slightly uphill to the main square of "+TOWN+".\n"+
"Directly north are the protective "+TOWN+" town gates.\n"+
"East is the entrance for "+SHOP+".\n"+
"A banner of the "+PROF_A+" is south of here.\n"+
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
          PATH+"road/ngates",AWAY,
	  PATH+"road/n2",TOWARDS,
          PATH+"room/shop","east",
   }));

}



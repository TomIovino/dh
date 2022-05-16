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
"East leads slightly uphill to the main square of "+TOWN+".\n"+
"Directly west are the protective "+TOWN+" town gates.\n"+
"North is the entrance for "+MSHOP+".\n"+
"A banner of the "+PROF_B+" is east of here.\n"+
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
          PATH+"road/wgates",AWAY,
	  PATH+"road/w2",TOWARDS,
          PATH+"room/mshop","north",
   }));

}



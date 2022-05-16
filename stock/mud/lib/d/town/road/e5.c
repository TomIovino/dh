#define AWAY "east"
#define TOWARDS "west"
#define ROAD_NAME "Eastern Road"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
   if(arg) return;
    set_short(ROAD_NAME);
    set_long(BROWN_F+
"Directly to the east the city gates can be seen leading into the great\n"+
"forest beyond. Northwest of here an entrance leads to the temple district.\n"+
"South along the road are walls that separate the market area from the\n"+
"rest of town.\n"+
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
    "Well worn cobblestones worked together to form the road.\n",
    "forest",GREEN_F"The huge world forest that covers the land.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    "gates",GREY_F"The large gates which allow traffic into the city.\n",
    "temples", RED_F"Various religious temples are northwest of here.\n",
   }));
   set_light(5);
   set_exits(({
	"d/town/road/egates",AWAY,
	"d/town/road/e4",TOWARDS,
   }));

}



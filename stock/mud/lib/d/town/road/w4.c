#define AWAY "west"
#define TOWARDS "east"
#define ROAD_NAME "Western Road"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
   if(arg) return;
    set_short(ROAD_NAME);
    set_long(BROWN_F+
"Close to the west the city gates can be seen leading into the great\n"+
"forest beyond. Dirctly southeast an entrance leads to the embassy\n"+
"district. North along the road are walls that separate the castle\n"+
"courtyard from the rest of town.\n"+
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
     "Well worn cobblestones worked together to form the road.\n",
    "forest",GREEN_F"The huge world forest that covers the land.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    "gates",GREY_F"The large gates which allow traffic into the city.\n",
    "castle",B_WHITE_F"A large castle in good repair to the northeast.\n",
   }));
   set_light(5);
   set_exits(({
	"d/town/road/w5",AWAY,
	"d/town/road/w3",TOWARDS,
   }));

}



#define AWAY "north"
#define TOWARDS "south"
#define ROAD_NAME "Northern Road"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
   add_object("citizen","d/town/npc/citizen_w");
   if(arg) return;
    set_short(ROAD_NAME);
    set_long(BROWN_F+
"To the north the city gates can be seen leading into the great\n"+
"forest beyond. Directly west of here an entrance leads to the castle\n"+
"courtyard. East along the road are walls that separate the temple\n"+
"district from the rest of town.\n"+
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
     "Well worn cobblestones worked together to form the road.\n",
    "forest",GREEN_F"The huge world forest that covers the land.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    "gates",GREY_F"The large gates which allow traffic into the city.\n",
    "temples", RED_F"Various religious temples are east of here.\n",
    "castle",B_WHITE_F"A large castle in good repair to the west.\n",
   }));
   set_light(5);
   set_exits(({
	"d/town/road/n4",AWAY,
	"d/town/road/n2",TOWARDS,
        "d/town/castle/ca","west",
     "/d/town/church/wchurch","east",
   }));

}



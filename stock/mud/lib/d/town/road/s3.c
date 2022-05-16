#define AWAY "south"
#define TOWARDS "north"
#define ROAD_NAME "Southern Road"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
   add_object("citizen","d/town/npc/citizen_w");
   if(arg) return;
    set_short(ROAD_NAME);
    set_long(BROWN_F+
"South of here the city gates can be seen leading into the great\n"+
"forest beyond. Directly east an entrance leads to the town's main\n"+
"market. West along the road are walls that separate the embassy\n"+
"district from the rest of town.\n"+
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
     "Well worn cobblestones worked together to form the road.\n",
    "forest",GREEN_F"The huge world forest that covers the land.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    "gates",GREY_F"The large gates which allow traffic into the city.\n",
    "market", GREEN_F"Various markets are east of here.\n",
    "castle",B_WHITE_F"A large castle in good repair to the northwest.\n",
   }));
   set_light(5);
   set_exits(({
	"d/town/road/s4",AWAY,
	"d/town/road/s2",TOWARDS,
        "d/town/market/ave1","east",
        "/d/town/embassy/emb1","west",
   }));

}



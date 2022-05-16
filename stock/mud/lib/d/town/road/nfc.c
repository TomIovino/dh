#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
   if(arg) return;
    set_short("North Wall Path");
    set_long(BROWN_F+
"This path follows the north city wall completely around. Towards the\n"+
"east of here you can see the northern gates and to the west of here\n"+
"is the large tower which is at the corner of the city walls. South of\n"+
"here is a smaller wall which protects the castle courtyard.\n"
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
    "Well worn cobblestones worked together to form the road.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    "gates",GREY_F"The large gates which allow traffic into the city.\n",
    "temples", RED_F"Various religious temples are northeast of here.\n",
    "castle",B_WHITE_F"A large castle in good repair to the northwest.\n",
   }));
   set_light(5);
   set_exits(({
	"d/town/road/nfd","east",
	"d/town/road/nfb","west",
   }));

}



#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
   if(arg) return;
    set_short("Southeast Tower");
    set_long(BROWN_F+
"These are the steps in front of the southeast tower of Twilite. A metal\n"+
"door with glyphs and wards is the entrance to the interior of the tower.\n"+
"Steps lead down from here to the path that circles the city proper.\n"+
   NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "door", B_GREEN_F+
    "Examining the door you recognize the elemental symbol of Water.\n",
    "d#down#ground#steps", BROWN_F+"The steps are made of carved stone.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
   }));
   set_light(5);
   set_exits(({
	"d/town/road/tow1","down",
   }));
   set_door_path("/d/town/tower/tow3");
   set_door_name("door");

}



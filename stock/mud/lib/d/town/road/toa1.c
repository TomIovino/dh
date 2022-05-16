#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
   if(arg) return;
    set_short("Southwest Tower");
    set_long(BROWN_F+
"This is the southwest corner of the city. A set of stairs on the\n"+
"wall lead up to a tower. North and east you can see paths that lead\n"+
"towards other parts of the city.\n"+ 
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
    "Well worn cobblestones worked together to form the road.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
   }));
   set_light(5);
   set_exits(({
	"d/town/road/saa","east",
	"d/town/road/waa","north",
   }));

}



// edited by Jada/10/98
#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
    if(arg) return;
    set_short("Northwest Castle Chamber");
    set_long(GREY_F+
"This chamber is well lit to allow activities such as reading and\n"+
"sparring. Castle hallways lead east and south to other chambers.\n"+
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
 
  "Well worn cobblestones worked together to form the road.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    }));
    set_exits(({
	"d/town/castle/cf","east",
	"d/town/castle/cd","south",
      }));
   set_light(5);
}

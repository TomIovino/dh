// edited by Jada/10/98
#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
    add_object("elite","d/town/npc/elite");
    if(arg) return;
    set_short("Castle Entrance");
    set_long(GREY_F+
"Directly before the entrance to the castle to the west. East of here\n"+
"is the city proper. Huge stone bricks have been laid here, and\n"+
"expertly fitted to form the castles exterior and a gate which is\n"+
 "open allows the only noticeable entrance into the castle.\n"+
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
  "Well worn cobblestones worked together to form the road.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    }));
    set_exits(({
	"d/town/castle/cb","west",
        "d/town/road/n3","east",
      }));
   set_light(5);



}



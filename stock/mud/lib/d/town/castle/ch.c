// edited by Jada/10/98
#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
    add_object("elite","d/town/npc/elite");
    if(arg) return;
    set_short("Southeast Castle Chamber");
    set_long(GREY_F+
"This chamber is well lit to allow activities such as reading and\n"+
"sparring. A castle hallway leads west into the south chamber.\n"+
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
  "Well worn cobblestones worked together to form the road.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    }));
    set_exits(({
	"d/town/castle/ci","west",
        "/d/town/castle/cb","north",
      }));
   set_light(5);
}

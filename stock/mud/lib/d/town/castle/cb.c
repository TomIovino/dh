//edited by Jada/10/98
#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
     add_object("elite","d/town/npc/elite");
    if(arg) return;
    set_short("Castle Foyer");
    set_long(GREY_F+
 "This chamber is well lit to allow activities such as reading and\n"+
 "sparring. East leads back to the castle's entrance, while halls\n"+
"lead west towards the castle center.\n"+
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
 "Well worn cobblestones worked together to form the road.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    }));
    set_exits(({
	"d/town/castle/ca","east",
	"d/town/castle/cc","west",
        "/d/town/castle/ch","south",
        "/d/town/castle/ce","north",
      }));
   set_light(5);
}

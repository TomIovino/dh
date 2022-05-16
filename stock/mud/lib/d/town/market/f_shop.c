#include <mudlib.h>
#include <ansi.h>
inherit ROOM;


void reset(status arg) {
    add_object("leo","d/town/npc/leo");
    if(arg) return;
    set_exits(({
	"d/town/market/ave4","south",
      }));
    set_short("The Tavern");
    set_long(RED_F"\
This is definitely the local's favorite hangout, the town pub. Citizens\n\
and adventurers wander in and out bragging about recent excursions outside\n\
the city walls. It's been said that if you're looking for adventuring work,\n\
the pub is the place to go. There is a menu tacked up behind the bar, and\n\
some stools situated around the bar itself.\n\
   ");
    set_light(5);
    set_items(({
      }));
}

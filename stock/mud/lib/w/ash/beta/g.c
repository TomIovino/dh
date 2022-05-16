#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
#include "hdefs.h"     
inherit ROOM;

void reset(status arg) {
    if(arg) return;
    add_object("guard1",PATH+HOUSE+"/guard");
    add_object("guard2",PATH+HOUSE+"/guard");
    add_object("guard3",PATH+HOUSE+"/guard");
    set_short(CASTLE+" gate");
    set_long(GREY_F+
    "An arched, tunnel-like entrance leads through the thick stone walls\n"+
    "into the interior of "CASTLE" Castle. Although the massive wooden\n"+
    "gates stand open, the castle doesn't appear welcoming to strangers.\n"+
    NORM);
    set_items(({
    "up#u#sky",
    "The grey stone walls of the castle rise high against the sky.\n",
    "down#d#ground",
    "The ground is mostly bare dirt with only a few patches of scruffy grass.\n",
    "castle",
    "This sturdy stone fortress belongs to House "CASTLE".\n",
    "walls#wall",
    "The walls of the castle are formed of massive blocks of grey stone.\n",
    "entrance",
    "An arched opening leads into the castle.\n",
    "gates#gate",
    "The heavy wooden gates are reinforced with iron.\n",
    "forest",
    "A thick forest surrounds the cleared area immediately outside the\n"+
    "castle.\n",
    }));
    set_smell("The smell of dry dusty earth tickles your nose.\n");
    set_listen("Faint birdsong can be heard from the surrounding forest.\n");
    set_exits(({
    PATH+HOUSE+"/e","north",
    PATH+HOUSE+"/h","south",
      }));
   set_light(5);

}

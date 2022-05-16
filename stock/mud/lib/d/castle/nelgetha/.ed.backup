#define TP this_player()
#include <ansi.h>
#include <mudlib.h>

#include "hdefs.h"

inherit ROOM;

void reset(status arg) {
    add_object("sentry",PATH+HOUSE+"/sentry");
    if(arg) return;
    set_short(CASTLE+" entrance");
    set_long(GREY_F+
    "The forest has been cut back to clear a large open area surrounding\n"+
    "the high stone walls of a sturdy castle. A large banner hangs above\n"+
    "the arched entrance, identifying the castle as belonging to House\n"+
    ""CASTLE". This looming fortress looks rather ominous to the uninvited,\n"+
    "giving the impression that strangers will be attacked on sight.\n"+
    NORM);
    set_items(({
    "up#u#sky",
    "The grey stone walls of the castle rise high against the sky.\n",
    "down#d#ground",
    "The ground is mostly bare dirt, with only a few patches of scruffy\n"+
    "grass.\n",
    "forest#area",
    "A thick forest surrounds the cleared area immediately outside the\n"+
    "castle.\n",
    "castle#fortress",
    "This sturdy stone fortress belongs to House "CASTLE".\n",
    "walls#wall",
    "The walls of the castle are formed of massive blocks of grey stone.\n",
    "entrance",
    "An arched opening leads into the castle.\n",
    "banner",
    "A large silk banner bearing the emblem of House "CASTLE" hangs above\n"+
    "the castle entrance.\n",
    "emblem",
    "YOUR DESCRIPTION GOES HERE.\n",
    }));
    set_smell("The smell of dry dusty earth tickles your nose.\n");
    set_listen("Faint birdsong can be heard from the surrounding forest.\n");
    set_exits(({
    PATH+HOUSE+"/g","north",
      }));
   set_light(5);

}


#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    add_object("slither","/w/ash/base/dragon/slither",1,1);
    if(arg) return;
    set_long(BLUE_F+
"In the den of the ice dragon.\n"
NORM);
    set_items(({
    "cave","Its dark.. and much darker north.\n",
    "ice#snow","Standard elements of a tundra cave.\n",
    }));
    set_light(1);
    set_short("chamber");
    set_smell("sniff.. decomposition?\n");
    set_listen("perhaps thats snoring?\n");
    set_exits(({
    PATH+"cave1","south",
              }));
}

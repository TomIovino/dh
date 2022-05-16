
#include "path.h"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;


void reset(status arg) {
    add_object("sentry",PATH+"sentry");
    if(arg) return;
    set_long(GREY_F+
"From this tower you can see over the keep walls and into the\n"+
"wilderness and even to the mountains beyond. This is obviously a\n"+
"good place for the guards to watch for invasions.\n"
  "");
    set_items(({
    "walls#wall",WALLS,
    "up#u#sky", SKY,
    "down#d#floor", ADJECTIVE+" stone floor.\n",
    "wilderness", WILD,
    "mountains", 
    "Large pointy fixtures composed of millions of tons of rock.\n",
    "tower","A high tower at the corner of the keep.\n",
    "keep","A small stone fortress.\n",
    }));
    set_light(1);
    set_short("Guard Tower");
    set_smell(SMELL);
    set_listen(LISTEN);
    set_exits(({
    PATH+"1a","down",
              }));
}

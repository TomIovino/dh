#include <ansi.h>
#include <mudlib.h>
#include <ansi.h>
#include "../element.h"

inherit ROOM;

void reset(status arg) {

    ::reset(arg);
    add_object("barkeep",PATH+"npc/barkeep");
    if(arg) return;
    set_short(PUB);
    set_exits(({
      PATH+"road/s2","west",
      }));
   set_long(COLOR+
"This is the "+PUB+".\n"+
"Many tables and chairs invite the passerby to sit and rest.\n"+
   NORM);
   set_light(5);  
}


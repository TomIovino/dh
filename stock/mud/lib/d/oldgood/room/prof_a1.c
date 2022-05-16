#include <mudlib.h>
#include <ansi.h>
#include "../element.h"
inherit ROOM;


void reset(status arg) {
    object obj;
    ::reset(arg);
    if(arg) return;
    add_object(PROF1,PATH+"npc/prof1");
    set_short(PROF_A);
    set_long(COLOR+
"This is the "+PROF_A+" Here you find the various\n"+
"tools of the "+PROF1+" profession. On the inner wall is a\n"+
"giant right handed "+RACE+" fist. This is the mark of the\n"+
"headquarters of the weaponsmasters of "+ELEMENT+".\n"+
    NORM);
    set_light(2);
    set_items(({
      }));
    set_exits(({
        PATH+"road/n2","east",
        PATH+"room/prof_a2","west",
      }));
   }


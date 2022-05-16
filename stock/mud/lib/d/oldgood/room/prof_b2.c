#include <mudlib.h>
#include <ansi.h>
#include "../element.h"
inherit ROOM;


void reset(status arg) {
    object obj;
    ::reset(arg);
    if(arg) return;
    add_object(PROF2,PATH+"npc/prof2_b");
    set_short(PROF_B);
    set_long(COLOR+
"This is the "+PROF_B+" Here you find the various\n"+
"tools of the "+PROF2+" profession. On the inner wall is a\n"+
"giant right handed "+RACE+" fist. This is the mark of the\n"+
"headquarters of the weaponsmasters of "+ELEMENT+".\n"+
    NORM);
    set_light(2);
    set_items(({
      }));
    set_exits(({
        PATH+"room/prof_b1","north",
      }));
   }


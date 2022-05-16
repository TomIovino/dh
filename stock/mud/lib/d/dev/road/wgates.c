#include <ansi.h>
#include <mudlib.h>
#include "../element.h"
inherit ROOM;

void reset(status arg) {
    add_object("guard1",PATH+"npc/guard");
    add_object("guard2",PATH+"npc/guard");
    add_object("guard3",PATH+"npc/guard");
    add_object("guard4",PATH+"npc/guard");
    add_object("guard5",PATH+"npc/guard");
    if(arg) return;
    set_type(ELEMENT);
    set_short("West Gates");
    set_long(COLOR+
"The massive west gates to the city of "+TOWN+" stand open to allow\n"+
"the people of "+ELEMENT+" to come and go freely. To the east the city\n"+
"becomes more populated as it leads upwards to the main square.\n"
OUT+"\n"+
    NORM);
   set_items(({
    "u#up#sky",COLOR+UP+".\n",
    "d#down#street#ground#road",COLOR+DOWN+".\n",
    "walls#gates#citywalls",COLOR+WALLS+".\n",
   }));
   set_smell(SMELL"\n");
   set_listen(SOUND"\n");
   set_light(5);
   set_exits(({
        PATH+"road/w3","east",
   }));
}


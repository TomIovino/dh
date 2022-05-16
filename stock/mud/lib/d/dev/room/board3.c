
#include <ansi.h>
#include <mudlib.h>
#include "../element.h"

inherit ROOM;

void reset(status arg) {
       if(!present("board")) {
        object bb;
        bb = clone_object(BOARD);
        bb->set_short(COLOR+capitalize(ELEMENT)+" Board");
        bb->set_imm_post(0);
        bb->set_long(COLOR+
"DO NOT POST HERE UNLESS YOU WILL BE MEMBER OF THIS REALM!!!!!!!\n"+
"This is where players may post notes of importance to the entire mud.\n"+
"All rules must be followed on boards, or you will be termianted.\n"+
        NORM);        
        bb->set_file(ELEMENT);
        move_object(bb,this_object());
     }
    if(arg) return;
    set_short(MEET);
    set_long(COLOR+
"This is the private chamber of "+MEET+".\n"+
"Here you can find notes from "+ELEMENT+" realm citizens.\n"+
    NORM);
    set_items(({
    "u#up#sky",COLOR"The chambers stone ceiling is above you.\n",
    "d#down#floor",COLOR"The chambers stone floor is below you.\n",
    "wall#walls",COLOR"The chambers stone ceiling is above you.\n",
    }));
    set_exits(({
	PATH+"road/e3","north",
      }));
   set_light(5);
}

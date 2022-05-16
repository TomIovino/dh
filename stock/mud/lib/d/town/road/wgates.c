#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
    add_object("guard1","d/town/npc/guard");
    add_object("guard2","d/town/npc/guard");
    add_object("guard3","d/town/npc/guard");
    if(arg) return;
    set_short("West Gates");
    set_long(BROWN_F+
"The massive west gates to the city of Twilite stand open to allow\n"+
"adventurers and merchants to come and go freely. In the east the\n"+
"great city unfolds. East and south of here is the entrance to the\n"+
"embassy district. East leads slightly uphill to the town square.\n"
    NORM);
   set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
     "Well worn cobblestones worked together to form the road.\n"+
    "forest",GREEN_F"The huge world forest that covers the land.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n"+
    "gates",GREY_F"The large gates which allow traffic into the city.\n",
    "castle",B_WHITE_F"A large castle in good repair to the northeast.\n",
   }));
   set_light(5);
   set_exits(({
       "d/world/l/13","out",
        "d/town/road/w5","east",
        "d/town/road/wfe","north",
        "d/town/road/wae","south",
   }));
}

closed(){
   if(!this_player()->query_security_level()){
   write("Guard says nobody is to leave town!\n");
   return 1; }
}


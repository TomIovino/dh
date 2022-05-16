#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
    add_object("guard1","d/town/npc/guard");
    add_object("guard2","d/town/npc/guard");
    add_object("guard3","d/town/npc/guard");
    if(arg) return;
    set_short("North Gates");
    set_long(BROWN_F+
"The massive north gates to the city of Twilite stand open to allow\n"+
"adventurers and merchants to come and go freely. In the south the\n"+
"great city unfolds. South and west of here the entrance to a large\n"+
"castle can be seen. South leads gently uphill to the town square.\n"
    NORM);
   set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
     "Well worn cobblestones worked together to form the road.\n"+
    "forest",GREEN_F"The huge world forest that covers the land.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n"+
    "gates",GREY_F"The large gates which allow traffic into the city.\n",
    "temples", RED_F"Various religious temples are southeast of here.\n",
    "castle",B_WHITE_F"A large castle in good repair to the southwest.\n",
   }));
   set_light(5);
   set_exits(({
        "d/town/road/n5","south",
        "d/town/road/nfe","west",
        "/d/world/m/12","out",
        "d/town/road/nee","east",
   }));
}

	
   closed(){
   if(!this_player()->query_security_level()){
   write("Guard says nobody is to leave town!\n");
   return 1; }
}


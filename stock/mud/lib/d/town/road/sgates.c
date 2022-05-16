#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
    add_object("guard1","d/town/npc/guard");
    add_object("guard2","d/town/npc/guard");
    add_object("guard3","d/town/npc/guard");
    if(arg) return;
    set_short("South Gates");
    set_long(BROWN_F+
"The massive south gates to the city of Twilite stand open to allow\n"+
"adventurers and merchants to come and go freely. To the north the\n"+
"great city unfolds. North and east of here the entrance to the market\n"+
"square can be seen. North leads gently uphill to the town square.\n"
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
         "/d/world/m/14","out",
          "d/town/road/s5","north",
          "d/town/road/sae","west",
          "d/town/road/swe","east",

   }));
}


closed(){
   if(!this_player()->query_security_level()){
   write("Guard says nobody is to leave town!\n");
   return 1; }
}


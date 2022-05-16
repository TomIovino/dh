#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
    add_object("daniel","d/town/npc/daniel");

    if(arg) return;
    set_short("Twilite Town Square");
    set_long(BROWN_F+
"This is the town square of the city of Twilite. In the center of the square\n"+
"is a statue of a woman pointing eastward towards the sky. A cobbled road\n"+
"forms a circle around the statue and leads off in various directions.\n"+
"This seems to be the highest ground in the city and provides an excellent\n"+
"view around Twilite. Northeast of here is the temple district, southeast of\n"+
"here is the market district, southwest is the embassy district while,\n"+
"the northwest is dominated by a Castle. City walls surround the city each\n"+
"corner having a large tower for protection.\n"+
  NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
     "Well worn cobblestones worked together to form the road.\n"+
    "forest",GREEN_F"The huge world forest that covers the land.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n"+
    "gates",GREY_F"The large gates which allow traffic into the city.\n",
    "temples", RED_F"Various religious temples are northeast of here.\n",
    "castle",B_WHITE_F"A large castle in good repair to the northwest.\n",
    "fountain#woman#dawn#statue", WHITE_F+
    "A large stone statue rises gracefully from the cobbled square\n"+
    "formed by the intersection of the streets. The center of the statue\n"+
    "is a woman, who looks down upon her city with kindness. At the base\n"+
    "of the statue is a plaque.\n",
    "plaque", "A bronze engraving is on it. You can 'read plaque' if you want.\n",
   }));
    set_exits(({
	"d/town/road/n1","north",
	"d/town/road/e1","east",
	"d/town/road/w1","west",
	"d/town/road/s1","south",
      }));
    set_light(5);
}

init() {
    ::init();
    this_player()->set_start();
    add_action("read","read");
}

read(str){
    if(str=="plaque" || str=="the plaque"){
	write("It reads, Praise Lady Dawn bringer of the Light. Carpe Diem.\n");
        return 1;
    }
    return 0;
}

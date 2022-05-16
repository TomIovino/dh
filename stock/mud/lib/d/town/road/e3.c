#define AWAY "east"
#define TOWARDS "west"
#define ROAD_NAME "Eastern Road"
#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {
   add_object("citizen","d/town/npc/citizen_w");
   if(arg) return;
    set_short(ROAD_NAME);
    set_long(BROWN_F+
"Towards the east the city gates can be seen leading into the great\n"+
"forest beyond. Directly north an entrance leads to the temple district.\n"+
"South along the road are walls that separate the market area from the\n"+
"rest of town.\n"+
    NORM);
   set_no_magic(1);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
    "Well worn cobblestones worked together to form the road.\n",
    "forest",GREEN_F"The huge world forest that covers the land.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    "gates",GREY_F"The large gates which allow traffic into the city.\n",
    "temples", RED_F"Various religious temples are north of here.\n",
   }));
   set_light(5);
   set_exits(({
	"d/town/road/e4",AWAY,
	"d/town/road/e2",TOWARDS,
        "d/town/church/church","north",
      "/d/town/market/n_ave3","south",
   }));

}


init() {
  ::init();
  add_action("north","north");
}

north() {
if(this_player()->query_alignment() == "good" ||
    this_player()->query_alignment() == "evil") {
  write("An invisible force stops you from going that way!\n");
  return 1; }
else
  this_player()->move_player("north#d/town/church/church.c");
return 1; }

#include <ansi.h>
#include <mudlib.h>
#include "path.h"

inherit ROOM;

int blah;
void reset(status arg) {
  blah = 0;
  if(arg) return;
  set_type(KI);
  set_short("Misty Mountain Foothills"); 
  set_long(GREY_F+
"A huge forbidding mountain looms directly to the west. A smooth\n"+
"cliff face meets sheer mountain outcroppings, providing a challenge\n"+
"to those who would continue up this rocky slope. Northwest and\n"+
"southwest are trails around the mountain peak.\n"+
   NORM);
   set_items(({
   "sky#up#u",
     "The sky is clear and the wind is steady, but the climb looks hard.\n",
   "mountain#peak",
     "An enormous granite mountain with a peak that rises steeply into\n"+
     "the sky.\n",
   "slope#wall#walls#cliff#face#outcropping#outcroppings",
     "The smooth granite walls of the mountain rise from sheer outcroppings\n"+
     "of rock that appear unscaleable.\n",
   "trail#trails",
     "Paths circle the peak of the mountain.\n",
   "down#d#floor#ground#tint#gravel#boulder#boulders",
     "Over the eons, boulders and small pieces of rock have rolled down\n"+
     "the mountain to make a graveled floor of crushed stone. The gravel\n"+
     "and boulders have a green tint from tiny flecks of emerald imbedded\n"+
     "in the stone.\n",
   }));
    set_light(1);
    set_smell("Clear air, cold and fresh.\n");
    set_listen("The sound of the wind whistling around the mountain.\n");
    set_exits(({
      "/w/ash/base/"+KI+"/up",        "up",
      "/w/ash/base/"+KI+"/rm3",       "northwest",
      "/w/ash/base/"+KI+"/rm1",       "southwest",
              }));
}

init(){
  ::init();
  add_action("climb","up");
}

climb(){  
        if(!random(5)  ) return 0;
          write("You slip and fall!\n"); 
          this_player()->adj_mp(-10);
          this_player()->adj_hp(-10);
          return 1;
}
       
         

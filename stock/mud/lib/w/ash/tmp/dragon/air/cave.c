#include <ansi.h>
#include <mudlib.h>
#include "path.h"

inherit ROOM;

void reset(status arg) {
  add_object("dragon","w/ash/dragon/"KI"/dragon",1,1);
  if(arg) return;
  set_type(KI);
  set_short("Warm Dark Cave");
  set_long(GREY_F+
"A dark cave hollowed from the mountain over eons and eons.\n"+
"Ancient stalagmites and stalactites around the sides of the\n"+
"cave twist the light and create shadows on the walls.\n"+
   NORM);
   set_items(({
   "stalagmites#stalactites",
     "Large rock formations hang down from the ceiling, and rise upward\n"+
     "from the floor.\n",
   "shadow#shadows",
     "Eerie shadows flicker on the cave walls.\n",
   "cave",
     "A large opening hollowed out of the mountain.\n",
   "mountain",
     "An ancient granite mountain.\n",
   "up#u#ceiling#wall#walls#side#sides#down#d#floor#ground",
     "The granite that forms the walls, ceiling and floor of the cave\n"+
     "has a yellow tint from tiny flecks of topaz imbedded in the stone.\n",
   }));
    set_light(0);
    set_smell("The air inside the cave has a foul, musty smell.\n");
    set_listen("The sound of the wind rushing past the cave opening.\n");
    set_exits(({
      "/w/ash/dragon/"+KI+"/up",  "out",
              }));
}

init(){
  ::init();
  add_action("out","out");
}


out(){ 
    if(present("dragon")){
       if(random(10)) return 0;
    }
write(YELLOW_F+"The dragon swoops his tail knocking you off your feet.\n"+NORM);
    this_player()->set_var("stun",10);
    this_player()->set_var("stun_time",1);
    return 1;
}

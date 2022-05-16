#include <ansi.h>
#include <mudlib.h>
#include "path.h"

inherit ROOM;

int blah;
void reset(status arg) {
  blah = 0;
  if(arg) return;
  set_type(KI);
  set_short("Misty Mountaintop");
  set_long(GREY_F+
"The air here at the peak is clouded with mist. What is for certain is that\n"+
"this is indeed the top of this mountain. In all directions the mountain\n"+
"declines rapidly in elevation. The exception being directly ahead in the\n"+
"form of a single large-mouthed cave opening.\n"+
   NORM);
   set_items(({
   "cave#opening#mouth",
     "The cave is dark, with waves of surprisingly warm air coming\n"+
     "from it.\n",
   "sky#up#u",
      "The sky is clear and the wind is steady.\n",
   "peak#mountain#top",
     "The peak of this enormous mountain stands out above the thick mist\n"+
     "that surrounds it like a moat.\n",
   "mist",
     "Tendrils of mist swirl up around the mountain peak from the\n"+
     "valley below.\n",
   "down#d#ground",
     "The ground slopes steeply down the mountainside toward the mist\n"+
     "shrouded valley below.\n",
    })); 
    set_light(1);
    set_smell("There is a faint foul odor coming from the cave.\n");
    set_listen("The sound of the wind whistling around the mountain.\n");
    set_exits(({
      "/w/ash/dragon/"+KI+"/rm1",  "down",
      "/w/ash/dragon/"+KI+"/cave", "cave",
              }));
}

       


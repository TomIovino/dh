#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

int blah;
void reset(status arg) {
  blah = 0;
  if(arg) return;
  set_short("Passage Overlook"); 
  set_long(GREY_F+
"Grey mist seeps into the area from the northeast in the direction of an\n"+
"ancient mountain range created long before the forests of the southwest.\n"+  
"The passage from the western forest lands ends here, and the ground slopes\n"+
"downward toward a small mist filled valley that would have to be navigated\n"+
"to reach the range.\n"+
   NORM);
   set_items(({
   "sky#up#u",
      "The sky is clear and the wind is steady.\n",
   "mist",
     "Tendrils of mist swirl up into the air from the valley below.\n",
   "passage",
      "A gravel passage between large mountain peaks.\n",
   "mountain#mountains#range",
      "Towards the northeast an entire range of mountains can be seen,\n"+
      "rising up from a thick mist that surrounds it like a moat.\n",
    "valley",
      "A deep valley filled with thick mist surrounds the base of the\n"+
      "mountains.\n",
    "forest#forests",
      "The large forested lands southwest of the mountains.\n",
    "ground#d#down#floor#gravel#rock#granite",
      "Over the eons, small pieces of rock have rolled down"+
      "\nthe mountain to make a graveled floor of crushed stone.\n",
   }));
    set_light(1);
    set_smell("Clear air, cold and fresh.\n");
    set_listen("You hear a muffled echo far off, perhaps an animal is hunting.\n");
    set_exits(({
      "/w/ash/dragon/path/pass_1a","west",
      "/w/ash/dragon/path/mist_start", "down",
              }));
}



init(){
  ::init();
  add_action("search","search");
}

search(string what){ 
      if(what != "gravel" || blah) return 0; 
      blah = 1;
      add_object("ore","/w/ash/dragon/path/stone"); 
      write("You see an interesting stone on the ground.\n");
      return 1; 
}
         

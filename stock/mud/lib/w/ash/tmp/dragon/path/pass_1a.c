#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

int blah;
void reset(status arg) {
  blah = 0;
  if(arg) return;
  set_short("Passage to the Mountains"); 
  set_long(GREY_F+
"Looming mountains climb high into the sky on both sides of this granite\n"+
"graveled passage. The intense shadow caused by the mountains suffocates\n"+
"light and brings coldness. To the northeast an entire range of mountains\n"+
"can be seen that is swallowed by a thick mist at its base.\n"+NORM);
   set_items(({
   "sky#up#u",
      "The sky is clear and the wind is steady.\n",
   "mountain#base#mountains#range#mist",
      "A granite mountain range centered to the northeast stands\n"+
      "out from a thick mist that surrounds it like a moat.\n", 
   "peak#peaks",
      "Towards the northeast an entire range of mountain peaks can be seen.\n",
    "passage#d#ground#down#floor#gravel#rock#granite",
      "Over the eons, small pieces of rock have rolled down"+
      "\nthe mountain to make a graveled floor of crushed stone.\n",
   "wall#walls",
     "Steep mountain walls overshadow the passage on both sides.\n",
   "shadow",
     "The deep shadow of the looming mountains makes this passage\n"+
     "cold and gloomy.\n",
   }));
    set_light(1);
    set_smell("Clear air, cold and fresh.\n");
    set_listen("You hear a muffled echo far off, perhaps an animal is hunting.\n");
    set_exits(({
      "/w/ash/dragon/path/pass_1b","east",
      "/d/world/xxx/111","forest",
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
         

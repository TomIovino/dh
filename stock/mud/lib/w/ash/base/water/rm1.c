#include <ansi.h>
#include <mudlib.h>
#include "path.h"

inherit ROOM;

int blah;
void reset(status arg) {
  blah = 0;
  if(arg) return;
  add_object("rune","w/ash/base/"+KI+"/rune",1,1);
  set_type(KI);
  set_short("Misty Mountain Foothills"); 
  set_long(GREY_F+
"A huge forbidding mountain looms directly to the north. A smooth\n"+
"cliff face meets sheer mountain outcroppings, providing a challenge\n"+
"to those who would continue up this rocky slope. Down leads into\n"+
"a frozen tundra. Northeast and northwest are trails around the\n"+
"mountain peak. The gravel and boulders of this area have a strange\n"+
"tint to them.\n"+
   NORM);
   set_items(({
   "sky#up#u",
      "The sky is clear and the wind is steady, but the climb looks hard.\n",
     "tundra","A frigid windswept wasteland.\n",
   "trails#trail",
     "Paths circle the peak of the mountain.\n",
   "slope#wall#walls#cliff#face#outcropping#outcroppings",
     "The smooth granite walls of the mountain rise from sheer outcroppings\n"+
     "of rock that appear unscaleable.\n",
   "mountain#peak",
     "An enormous granite mountain with a peak that rises steeply into\n"+
     "the sky.\n",
   "down#d#floor#ground#tint#gravel#boulders#boulder",
     "Over the eons, boulders and small pieces of rock have rolled down\n"+
     "the mountain to make a graveled floor of crushed stone. The gravel\n"+
     "and boulders have a blue tint from tiny flecks of sapphire imbedded\n"+
     "in the stone.\n",
   }));
    set_light(1);
    set_smell("Clear air, cold and fresh.\n");
    set_listen("The sound of the wind whistling around the mountain.\n");
    set_exits(({
      "/w/ash/base/"+KI+"/up",        "up",
      "/w/ash/base/"+KI+"/rm2",       "northwest",
      "/w/ash/base/"+KI+"/rm4",       "northeast",
      "/w/ash/base/tundra/2c","down",
              }));
}

init(){
  ::init();
  add_action("search","search");
  add_action("climb","up");
    if(this_player()->query_race() == "dragon")
  command("nw",this_player());

}

climb(){  
        if(!random(10)  ) return 0;
          write("You slip and fall!\n"); 
          this_player()->adj_mp(-10);
          this_player()->adj_hp(-10);
          return 1;
}
       

search(string what){ 
      if(random(3)) blah = 1;
      if(what != "gravel" || blah) return 0; 
      blah = 1;
      add_object("rune","/w/ash/base/"+KI+"/rune"); 
      write("You see an interesting stone on the ground.\n");
      return 1; 
}
         

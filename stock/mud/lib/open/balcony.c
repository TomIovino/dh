#include <mudlib.h>
inherit "inherit/room2";
 
void reset(status arg) {
  if(!present("dove")) {
move_object(clone_object("/players/arthur/tower_dove"),this_object());
}
  if(arg) return;
  set_short("Tower Balcony");
  set_long(
"This is the balcony of the glass tower. You can see all of King Arthur's\n"+
"castle grounds spread out before you, and beyond the castle ground you can\n"+
"see the village of Tintagel. The balcony on which you stand is made of stone,\n"+
"which cannot possible be supported by the glass tower, but some how is. In\n"+
"the corner of the railing is a dove's nest. The balcony is about waist high,\n"+
"so one could easily steup over it, if one really wanted to. To the south is \n"+
"a door which leads back into the glass tower.\n");
  set_light(3);

load_door(({  
    "destination",       "players/arthur/tower",
    "direction",         "south",
    "closed",            
    "description",       "The door is made of glass, and leads south to the glass tower.\n",


  set_items(({
     "floor#balcony#rail",
          "The stone balcony has a railing, which stands about waist high, and could easily be climbed over if need be.",
     "nest#dove's nest",
          "The nest of a dove sits on the railing. It is made of twigs and leaves.",
     "ground#village#grounds#down",
          "You can see the village of Tintagel, and the grounds of King Arthur's castle over the rail.",
    
  }));
  set_exits(({
    "players/arthur/tower","south",
  }));
}

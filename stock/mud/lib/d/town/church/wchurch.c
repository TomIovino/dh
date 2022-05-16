#include <mudlib.h>
#include <ansi.h>
inherit CHURCH;


void reset(status arg) {
    object obj;

    ::reset(arg);
    if(arg) return;
    set_short("Temple of the Divine");
    set_long(CYAN_F"\
A huge octagonal room that is constructed of white marble. The upper\n\
gallery consists entirely of windows that are filled with stained glass\n\
which allows the bright sun to turn the marble into jewel-like colors.\n\
Echoes repeat themselves over and over creating a soft murmur that is\n\
constantly renewing itself, almost as if the room was speaking. Slender\n\
columns rise from the ground at each corner of the octagon, framing eight\n\
crystalline statues of dragons at each of the corners of the room.\n\
  ");

    set_no_fight(1);
    set_no_magic(1);
    set_no_steal(1);

    set_light(2);
    set_items(({
	"marble#walls",
	"White marble which glows with an inner light and is colored by the jewelled\n"+
	"tones of the stained glass windows.\n",
	"windows#stained glass#stained glass windows#light",
	"Huge stained glass windows depicting various good deeds by the founders of\n"+
     "fair Twilite spill colored light across the white marble room.\n",
	"floor","A white marbled floor that has been worn by many feet.\n",
	"statues#dragons",
	"Crystalline statues of dragons in various poses stand in each corner of the\n"+
        "room, watching over the citizens of Twilite.\n",
	"ceiling#gallery",
	"The ceiling rises high above the gallery of windows, forming a steepled\n"+
	"pinnacle at the center of the room.\n",
      }));
    set_exits(({
          "/d/town/road/n3","west",
         "/d/town/church/pboard","east",
      }));
}



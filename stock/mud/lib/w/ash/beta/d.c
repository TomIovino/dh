#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
#include "hdefs.h"     
inherit ROOM;

void reset(status arg) {
    if(arg) return;
    set_short(CASTLE+" west corridor");
    set_long(GREY_F+
    "Flickering torches light the corridor that runs between the main hall\n"+
    "and the Regent's quarters. The thick stone walls mute the sounds of\n"+
    "activity in other parts of the castle, reducing them to an echoing\n"+
    "murmur. A narrow window at the bend of the corridor looks out onto\n"+
    "the forest beyond the grounds of the keep.\n"+
    NORM);
    set_items(({
    "u#up#ceiling",
    "The high stone ceiling is supported by thick wooden crossbeams.\n",
    "d#down#floor",
    "The stone floor has been worn smooth by the traffic of booted feet.\n",
    "walls#wall#torches#torch",
    "Torches are mounted on the thick stone walls to provide light.\n",
    "corridor#bend",
    "This narrow hallway takes a sharp turn as it runs between the main\n"+
    "hall and the Regent's quarters.\n",
    "castle#keep",
    "This sturdy stone fortress belongs to House "CASTLE".\n",
    "window",
    "The window allows a view outside the castle, but is too narrow to\n"+
    "allow entry.\n",
    "forest#grounds",
    "A thick forest surrounds the cleared area immediately outside the\n"+
    "castle.\n",
    }));
  set_smell("The smell of fresh outside air drifts in through the window.\n");
  set_listen("Faint sounds from other parts of the castle echo through the\n"+
  "corridor.\n");
    set_exits(({
    PATH+HOUSE+"/a","north",
    PATH+HOUSE+"/e","east",
      }));
   set_light(5);

}

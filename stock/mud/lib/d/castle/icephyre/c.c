#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
#include "hdefs.h"     
inherit ROOM;

void reset(status arg) {
    if(arg) return;
    set_short(CASTLE+" Ambassador's quarters");
    set_long(GREY_F+
    "The Ambassador's room is plain and sparsely furnished, containing a narrow\n"+
    "cot on one side of the room, with a wooden desk and chair opposite. A\n"+
    "large fireplace occupies the wall at the end of the room. A golden\n"+
    "plaque hangs above the mantel, embossed with the emblem of House\n"+
    ""CASTLE".\n"+
    NORM);
    set_items(({
    "u#up#ceiling",
    "The high stone ceiling is supported by thick wooden crossbeams.\n",
    "d#down#floor",
    "The stone floor has been worn smooth by the traffic of booted feet.\n",
    "walls#wall#torches#torch",
    "Torches are mounted on the thick stone walls to provide light.\n",
    "castle#keep",
    "This sturdy stone fortress belongs to House "CASTLE".\n",
    "cot",
    "The Ambassador's bed is a simple wooden cot.\n",
    "desk#chair",
    "A plain wooden desk and chair provide a place for the Ambassador to\n"+
    "conduct house business.\n",
    "fireplace",
    "A fire burns in the large stone fireplace, warming the room.\n",
    "plaque#emblem",
    "YOUR DESCRIPTION GOES HERE.\n",
    "mantel",
    "A rough wooden plank serves as a shelf above the fireplace.\n",
    }));
  set_smell("You smell the scent of wood burning in the fireplace.\n");
  set_listen("Faint echoes of sound can be heard from other parts of the castle.\n");
  set_pk_only(1);    
    set_exits(({
    PATH+HOUSE+"/f","south",
      }));
   set_light(5);

}

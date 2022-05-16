#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
#include "hdefs.h"     
inherit ROOM;

void reset(status arg) {
    if(arg) return;
    set_short(CASTLE+" Regent's quarters");
    set_long(GREY_F+
    "The Regent's room has a black leather couch on one side of the room,\n"+
    "with a green plush sofa chair and a huge desk covered with papers on\n"+
    "the opposite side. An unusual vase sits on a table in the center of the\n"+
    "room. Posters decorate the walls, and a painting of a darkelf and a\n"+
    "lich hangs above the mantel, right below a golden plaque embossed with\n"+
    "the emblem of House "CASTLE". The room is fully clouded with smoke.\n"+
    NORM);
    set_items(({
    "u#up#ceiling",
    "The high stone ceiling is supported by thick wooden crossbeams.\n",
    "d#down#floor",
    "The stone floor has been worn smooth by the traffic of booted feet.\n",
    "walls#wall#torches#torch",
    "Torches are mounted on the thick stone walls to provide light.\n",
    "couch","A large black leather couch.\n",
    "chair","Tritian's large soft comfortable sofa chair.\n",
    "desk#paper#papers",
    "The top of the large wooden desk is littered with papers.\n",
    "vase#table",
    "A strange glass vase with tubes coming out of it sits on a plain\n"+
    "wooden table in the center of the room.\n",
    "smoke","Thick smoke hangs in the air of the room.\n",
    "posters#poster","The posters are drawings of plants.\n",
    "painting#minotaur#lich",
    "A formal painting of Tritian and Kayla in full battle armour.\n",
    "castle#keep",
    "This sturdy stone fortress belongs to House "CASTLE".\n",
    "cot",
    "The Regent's bed is a simple wooden cot.\n",
    "fireplace",
    "A fire burns in the large stone fireplace, warming the room.\n",
    "emblem",
    ""EMBLEM".\n",
    "plaque","A large golden plaque bearing the house emblem.\n",
    "mantel",
    "A rough wooden plank serves as a shelf above the fireplace.\n",
    }));
  set_smell("You smell the scent of wood burning in the fireplace.\n");
  set_listen("Faint echoes of sound can be heard from other parts of the castle.\n");
  set_pk_only(1);

    set_exits(({
    PATH+HOUSE+"/d","south",
      }));
   set_light(5);

}




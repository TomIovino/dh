#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
#include "hdefs.h"     
inherit ROOM;

void reset(status arg) {
    add_object("guardian",PATH+HOUSE+"/guardian");
    if(arg) return;
    set_short(CASTLE+" treasury");
    set_long(GREY_F+
    "This stone room in the depths of the castle serves as the treasure room\n"+
    "for House "CASTLE", a place for the House to store the plunder they\n"+
    "acquire from battles and raids. There is a small note attached to one\n"+
    "of the walls.\n"+
    NORM);
     set_no_clean(1);
    set_items(({
    "u#up#ceiling",
    "The high stone ceiling is supported by thick wooden crossbeams.\n",
    "d#down#floor",
    "The stone floor has been worn smooth by the traffic of booted feet.\n",
    "walls#wall#torches#torch",
    "Torches are mounted on the thick stone walls to provide light.\n",
    "castle#keep",
    "This sturdy stone fortress belongs to House "CASTLE".\n",
    "note",
    "Items may be left here during the course of a boot. The mud assumes\n"+
    "no responsibility for items lost due to a sudden reboot or crash.\n"+
    "No reimbursements!\n",
    }));
  set_smell("The air is dry and musty.\n");
  set_listen("The slightest sounds echo around the stone walls.\n");
    set_exits(({
    PATH+HOUSE+"/b","south",
      }));
   set_light(5);

}

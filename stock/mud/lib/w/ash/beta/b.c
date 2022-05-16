#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
#include "hdefs.h"     
inherit CHURCH;

void reset(status arg) {
       if(!present("board")) {
        object bb;
        bb = clone_object(BOARD);
        bb->set_short(CASTLE+" board");
        bb->set_imm_post(1);
        bb->set_long(
  "This board belongs to House "CASTLE".\n"+NORM+
   "\n");
        bb->set_file(HOUSE);
        move_object(bb,this_object());
     }

    if(arg) return;
    set_short(CASTLE+" chapel");
    set_long(GREY_F+
    "The highly polished marble walls of the "CASTLE" chapel rise to a\n"+
    "domed ceiling far overhead. Large white candles in tall stands are\n"+
    "placed around the room, shedding a soft, warm light that is conducive\n"+
    "to prayer and meditation.\n"+
    NORM);
    set_no_fight(1);
    set_no_magic(1);
    set_no_steal(1);
    set_items(({
    "up#u#ceiling",
    "The high domed ceiling is lost in shadows as it arches high overhead.\n",
    "down#d#floor",
    "The stone floor has been worn smooth by the traffic of booted feet.\n",
    "walls#wall",
    "The light from the candles reflects off of the polished marble walls.\n",
    "candles#candle#stands#stand",
    "The room is lit by large thick candles that are mounted in tall iron\n"+
    "stands.\n",
    "chapel",
    "This peaceful room gives the members of House "CASTLE" a place to pray\n"+
    "and meditate.\n",
    }));
    set_smell("The scent of incense hangs in the air.\n");
    set_listen("Faint whispers seems to echo around the room.\n");
    set_exits(({
    PATH+HOUSE+"/e","south",
    PATH+HOUSE+"/i","north",
      }));
   set_light(5);

}

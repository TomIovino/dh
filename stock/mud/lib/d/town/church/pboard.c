#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
inherit CHURCH;

void reset(status arg) {
  if(0)
       if(!present("board")) {
        object bb;
        bb = clone_object(BOARD);
        bb->set_short(B_CYAN_F+"Dragonheart Discussion Board"+NORM);
        bb->set_imm_post(0);
        bb->set_long(B_BLUE_F+
   "This board contains notes from fellow players.\n"+
   "ALL RULES APPLY ON BOARDS!!\n"+
   "All discussion within the realms of the rules are welcomed here.\n");
        bb->set_file("players");
        move_object(bb,this_object());
     }
   if(arg) return;
    set_no_fight(1);
    set_no_magic(1);
    set_no_steal(1);

   set_short("Below the Board Room");
   set_type("neutral");
    set_long(CYAN_F+
"A cold stonewalled room in the back of the church.\n"+
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
  "Well worn cobblestones worked together to form the road.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    }));
    set_exits(({
          "/d/town/church/wchurch","west",
         "/d/town/church/aboard","east",
      }));
   set_light(5);
}
/*
init(){
  ::init();
  if(this_player()->query_level() < 50){
       command("south",this_player());
  }
}
*/

#define TP this_player()
#include <ansi.h>
#include <mudlib.h>
inherit CHURCH;

void reset(status arg) {
       if(!present("board")) {
        object bb;
        bb = clone_object(BOARD);
        bb->set_short(B_RED_F+"Divine Guidance Board"+NORM);
        bb->set_imm_post(1);
        bb->set_long(B_BLUE_F+
  "This board contains announcements from the game admins.\n"+
   "\n");
        bb->set_file("announcement");
        move_object(bb,this_object());
     }
   if(arg) return;
    set_no_fight(1);
    set_no_magic(1);
    set_no_steal(1);

   set_short("Announcement Board Room");
   set_type("neutral");
    set_long(CYAN_F+
"A cold stonewalled room in the back of the twilite church.\n"+
    NORM);
    set_items(({
    "u#up#sky",BLUE_F"A few clouds float high above you.\n",
    "d#down#street#ground#cobblestones#dirt#road", BROWN_F+
  "Well worn cobblestones worked together to form the road.\n",
    "walls#citywalls",GREY_F"The walls protect portions of the city.\n",
    }));
    set_exits(({
         "/d/town/church/donation","south",
       "/d/town/church/pboard","west",
      }));
   set_light(5);
}
init(){
  ::init();
}

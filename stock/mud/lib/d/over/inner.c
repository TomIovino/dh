#include <ansi.h>
#include <mudlib.h>
inherit ENV;

void reset(status arg) {
object board;

    if(!present("board")) {
        object bb;
        bb = clone_object(BOARD);
      bb->set_short(B_CYAN_F+"Immortal Announcement Board"+NORM);
        bb->set_long(
          "This board contains posts of general interest to wizards working\n"
          "on Crimsonesti. Please check this board regularly and feel free\n"
          "to post any questions, concerns, or comments here.\n"
           );
        bb->set_file("immortal");
        bb->set_max_notes(100);
        move_object(bb,this_object());
    }

  if(arg) return;
 set_short("Heaven 101");
  set_long(CYAN_F+
  "This is where builders can post notes of an announcement nature\n"+
  "for other builders it should be checked every time you log in.\n"+
     " ");
  set_exits(({
  "/d/over/portal","down",
  })); 
  set_light(2);
}

////////////////////////////////////////////////////////////
// ARENA.C           // DRAGONHEART // ASH.7.01.98        //
////////////////////////////////////////////////////////////


#include <ansi.h>
#include <mudlib.h>
inherit ENV;
#define TP this_player() 

void reset(status arg) {
  ::reset(arg);
  if(arg) return;
  set_short("Arena");
  set_long(GREY_F+
"The mirrored hallway\n");
  set_no_summon(1);
  set_no_steal(1);
  set_no_track(1);
  set_light(10);
  set_exits(({
   "/d/town/temple/church","down",
  }));
  }

void init(){
  ::init();
      write("Your losing energy from running during the battle!\n");
      this_player()->adj_hp(-5);
      if(this_player()->query_hp() == 0) this_player()->death();
      this_player()->set_hiding(0);
      if(this_player()->query_invis()){
      this_player()->toggle_invis();
      this_player()->adj_hp(-5);
    }
      this_player()->dispel_magic();
  }


  

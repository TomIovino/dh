////////////////////////////////////////////////////////////
// ARENA.C           // DRAGONHEART // ASH.7.01.98        //
////////////////////////////////////////////////////////////


#include <ansi.h>
#include <mudlib.h>
inherit ARENA;
#define TP this_player() 

void reset(status arg) {
  ::reset(arg);
  if(arg) return;
  set_short("Arena");
  set_long(GREEN_F+
"The grass is matted down here from aparently over use of some sort.\n"+
"From this spot you can leave into other parts of the hedge garden.\n");
  set_light(10);
  set_exits(({
  }));
  }

void init(){
  ::init();
  }


  

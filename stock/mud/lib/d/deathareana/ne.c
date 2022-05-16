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
"The path follows around here to the west and to the south. Huge\n"+
"green hedges form the path that prevents any other direction.\n");
  set_light(10);
  set_exits(({
  "/d/arena/n","west",
  "/d/arena/e","south",
  }));
  }

void init(){
  ::init();
  }


  

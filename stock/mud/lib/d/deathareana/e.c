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
"The path follows around here to the north and to the south.\n"+
"West is an entrance into an area of matteted down grass.\n");
  set_light(10);
  set_exits(({
  "/d/arena/main","west",
  "/d/arena/ne","north",
  "/d/arena/se","south",
  }));
  }

void init(){
  ::init();
  }


  

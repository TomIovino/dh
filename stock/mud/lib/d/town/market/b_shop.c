#include <ansi.h>
#include <mudlib.h>
inherit ROOM;

void reset(status arg) {

    ::reset(arg);
    add_object("amber","/d/town/npc/amber");
    if(arg) return;
 set_short("First Bank of Twilite");
    set_exits(({
      "/d/town/market/ave3","north",
      }));
    set_long(RED_F+
"The walls of this shop are lined with various recorded keeping items.\n"+
"Type 'help bank' for a list of commands used for keeping money here.\n"+
    NORM);
    set_light(5);
}

  init(){
   ::init();
   add_action("list","list");
   }
 list(){ command("help bank",this_player()); return 1; }





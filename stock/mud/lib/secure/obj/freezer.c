////////////////////////
// The Freezer -- Where linkdead people go...
// Idea from Nightmare
////////////////////////

#include <ansi.h>
#include <mudlib.h>
inherit ROOM;
 
void reset(int arg) {
  if (arg) return;
  set_no_fight(1);
  set_no_summon(1);
  set_no_track(1);
  set_short("The Freezer");
  set_long(BOLD+BLUE_F"\
Brrrrr is cold! (if you see this RELOG)\n\
");
  set_light(2);
}

void init() {
   ::init();
  catch("/secure/login_tracker_d"->logoff(name,1));
   add_action("WakeUp","",1);  
   add_action("hmm","l");
   add_action("hmm","look");
}
// goto_enter fun is in transport.h

hmm(){ this_player()->goto_enter(); }
status WakeUp(string arg) {
    this_player()->reconnect();
    return 0;
}



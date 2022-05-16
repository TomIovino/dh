


#define TP this_player()
#include <ansi.h>
#include <mudlib.h>

inherit MONSTER;

string rider;
object owner;



 reset(arg) {
    if(arg) return;
    set_name("horse");
    set_race("horse");
    set_level(10);
 }


 init(){
  ::init();
  add_action("mount","mount");
  add_action("DISmount","dismount");
 }


 mount(){
    owner = this_player();
    write("You mount the "+query_name()+".\n");
    say(owner->query_name()+" mounts the "+query_name()+".\n");
    move_object(this_object(), this_player());
    return 1;
 }
 DISmount(){
    owner = this_player();
    write("You dismount the "+query_name()+".\n");
    say(owner->query_name()+" dismounts the "+query_name()+".\n");
    move_object(this_object(), environment(this_player()));
    return 1;
 }

 short(){ if(environment() == this_player()) return 0;
          return ::short();
        }

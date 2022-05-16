/* scryer object */

#include <mudlib.h>
#include <ansi.h>
inherit "inherit/timer";
inherit TREASURE;


object old_room, master, eye_controller;

void reset(status arg) {
  if(arg) return;
  set_name("scan");
  set_short("scan");
  set_long("scan");
}

string short(string wiz){ return 0; }
status query_npc() { return 1; }

void scan(object who) {
  if(!who) {
    destruct(this_object());
    return;
  }
  master = who;
}


int query_dead(){ return 1; }

void new_room(string str) {
  object ob;
  string *exits;
  int i;

  if(!str) return;
  if(!(master || eye_controller)) {
    time_out();
    return;
  }
  if(!old_room) old_room = environment();
  exits = (string *)environment()->query_open_exits();
  if(member_array(str, exits) != -1) {
    for( i = 0; i < sizeof(exits); i++) {
      if(exits[i] == str) {
        move_object(this_object(), exits[i-1]);
      }
    }
    str = "l";
  }
  if(old_room != environment() ||
    str == "l" || str == "look") {
    if(environment()->query_no_track()) {
      write(B_WHITE_F+"You can't see clearly in that direction.\n"+NORM);
      destruct(this_object());
      return; }
    environment()->long(0);
    this_player()->show_inventory(environment());
    old_room = environment();
      destruct(this_object());
    return;
  }  
}



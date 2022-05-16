/* scryer object */

#include <mudlib.h>
#include <ansi.h>
inherit "inherit/timer";
inherit NPC;


object old_room, master, eye_controller;

void reset(status arg) {
  NPC::reset();
  if(arg) return;
  set_name("eye");
  set_alt_name("spell");
  set_short(B_CYAN_F+"explorer eye"+NORM);
  set_long("\
The eye is transluscent, and wavers about ten feet above the ground. \n\
It is without a doubt magical.\n");
  set_living_name("eye");
  enable_commands();
  set_level(10);
  set_magic_resist(1000);
}

status query_npc() { return 1; }

int query_dead(){ return 1; }

void eye(object who) {
  if(!who) {
    destruct(this_object());
    return;
  }
  master = who;
}


void time_out() {
  if(master) {
    tell_object(master,"The explorer eye dispels.\n");
  }
  if(eye_controller) destruct(eye_controller);
  ::time_out();
}

void heart_beat() {
  if(this_object()->query_primary_attack()) {
    time_out();
    return;
  }
}

void dispel_magic() { time_out(); }


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
   if(environment()->query_pk_only() || environment()->query_no_track()){
      write(B_WHITE_F+"You are blinded as your explorer eye suddenly disintegrates in a flash of white!\n"+NORM);
      destruct(this_object());
      return; }
    write(
"#################################################################\n"+
"Eye->\n");
    environment()->long(0);
    this_player()->show_inventory(environment());
    write("\n"+
"#################################################################\n");
    old_room = environment();
    return;
  }  
  if(sscanf(str,"look at %s",str)) {
    if(!environment()->look_at(str)) {
      if(ob = present(str, environment())) {
        write(
"#################################################################\n"+
"Eye->\n");
        ob->long(0);
        write(
"\n#################################################################\n");

      }
      else {
        write(
"#################################################################\n"+
"Eye->There is no "+ str +" here.\n\n"+
"#################################################################\n");
      }
    }
    return;
  }
  write("###Scrying Eye->You refocus the eye.###\n");
}


void catch_tell(string str) {
  if(!(master || eye_controller)) {
    time_out();
    return;
  }
  if(environment(master) != environment()) {
    tell_object(master,
"#################################################################\n"+
"Eye->\n"+ str +"\n"+
"#################################################################\n");
  }
}




status cast_spell(object caster,object target,object prev,int level) {
  object ob;
  string who;

  if((ob = present("scrying eye", caster))) {
    if(ob->query_wizard_eye()) {
      ob = (object)ob->query_wizard_eye();
      write("You extend the time of your scrying eye spell.\n");
      ob->adj_time(level*3);
      destruct(this_object());
      return 1;
    }
    else {
      destruct(ob);
    }
  }
  eye_controller = clone_object("inherit/objs/explorer_obj");
  eye_controller->set_wizard_eye(this_object());
  move_object(eye_controller, caster);
  master = caster;
  adj_time(level*3);
  printf("\
The area flashes brightly momentarily and a large floating eye appears.\n");
  return 1;
}


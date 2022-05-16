#include <ansi.h>
#include <mudlib.h>

inherit BASE;
#define COLOR  ({ BROWN_F,MAGENTA_F,BLUE_F,RED_F,GREEN_F,YELLOW_F,CYAN_F,})


object owner;
int wc;
int c;
string color;

string short(string x){
  c = random(sizeof(COLOR));
  color = COLOR[c];
  return color+"ethereal defender shield"+NORM;
}


void reset(status arg) {
  if(arg) return;
  owner = this_player();
  set_name("defender");
  set_smell("You smell the odour of ozone as it crackles off the blades.\n");
  set_listen("You hear it hum softly.\n");
  set_short(owner->query_name() + "'s "+
            "defender shield"+NORM);
  set_long("The blade shimmers with a magical aura.\n"+
           "Upon its handle are written magical runes.\n");
  set_read("You cannot read the runes of power!\n");
  set_extra_info("This shield has a life of its own. It won't let you\n"+
                 "touch it.\n");
}

void heart_beat() {
  object attack_ob;
  int dmg;

  if(!owner) { /* logged out */
     destruct(this_object());
     return;
  }
  if(!present(owner, environment(this_object()))) {
    say("The defender follows its owner.\n");
    move_object(this_object(), environment(owner));
    tell_room(environment(owner), "The defender floats into the room.\n");
  }
}


void set_owner(object ob) { 
  owner = ob;
  if(ob) {
    set_heart_beat(1);
    set_info("The shield was enchanted by "+(string)ob->query_name()+".\n");
    wc = (int)ob->query_level()/2;
  }
}


object query_owner() { return owner; }


void dispel_magic() { 
   string ob;
  if(owner) {
    ob = owner->query_varible("defender");
    if(ob) {
    destruct(this_object());
    }
  }
}



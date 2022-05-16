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
  return color+"ethereal defender sword"+NORM;
}


void reset(status arg) {
  if(arg) return;
  owner = this_player();
  set_name("sword");
  set_smell("You smell the odour of ozone as it crackles off the blades.\n");
  set_listen("You hear it hum softly.\n");
  set_short(owner->query_name() + "'s "+
            "avenger sword"+NORM);
  set_long("The blade shimmers with a magical aura.\n"+
           "Upon its handle are written magical runes.\n");
  set_read("You cannot read the runes of power!\n");
  set_extra_info("This sword has a life of its own. It won't let you\n"+
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
    say("The avenger follows its owner.\n");
    move_object(this_object(), environment(owner));
    tell_room(environment(owner), "The avenger floats into the room.\n");
  }
  attack_ob = (object)owner->query_attack();  
  if(attack_ob) {
    string nam;

    if(present(attack_ob, environment(owner))) {
      dmg = random(wc+1*3) - random((int)attack_ob->query_ac()+1);
      if(dmg > 25) dmg = 25;
         dmg = random(dmg);
      if(dmg < 1) dmg = 1;
      if(dmg < (int)attack_ob->query_hp()) {
        attack_ob->adj_hp(-dmg); /* no fight ensues */
      }
      nam = (string)attack_ob->query_name();
      tell_room(environment(),"The sword "+
      ((dmg > 21)
      ? "gashed "+nam+" spilling blood everywhere"
      : (dmg > 18)
      ? "sliced "+nam+" across the throat"
      : (dmg > 15)
      ? "pierced "+nam+" in the abdomen"
      : (dmg > 12)
      ? "cut "+nam+" on the arm"
      : (dmg > 9)
      ? "chopped "+nam+" on the leg"
      : (dmg > 6)
      ? "grazed "+nam+" on the foot"
      : (dmg > 3)
      ? "brushed "+nam+" on the arm"
      : (dmg > 0)
      ? "hurt "+nam+" on the hand"
      : "missed "+nam)+".\n");
    }
  }
}


void set_owner(object ob) { 
  owner = ob;
  if(ob) {
    set_heart_beat(1);
    set_info("The sword was enchanted by "+(string)ob->query_name()+".\n"+
             "It will expire in about @@query_approx_time:"+
              file_name(previous_object())+"@@.\n");
    wc = (int)ob->query_level()/2;
  }
}


object query_owner() { return owner; }


void dispel_magic() {
  object ob;

  if(owner) {
    ob = present("spellsword",owner);
    if(ob) {
      ob->dispel_magic(); /* destructs this */
      return;
    }
  }    
  destruct(this_object());
}



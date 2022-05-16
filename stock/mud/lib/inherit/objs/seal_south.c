/* barrier seal */

#include <ansi.h>
#define QW "south"

string test;
int strength;

status cast_spell(object caster,object target,object prev,int dmg) {
  strength = caster->query_skill("barrier seal")/10;
  tell_room(target, "A barrier seal is "+QW+" of here.\n");
  return 1;

}  
status id(string str) {
  return str == "spell"
      || str == "magic"
      || str == "seal"
      || str == "barrier seal";
}

init(){
   if(present("barrier seal 2",environment())) destruct(this_object());
  add_action("move",QW);
  }

int move(){
     strength--;
     if(strength < 0){
     this_object()->dispel_magic();   
     return 0;
     }
     write(YELLOW_F+"You run into a barrier!!\n"+NORM);
     say(YELLOW_F+this_player()->query_alt_name()+
                  " runs into a barrier!\n"+NORM);
     return 1;
}

void dispel_magic() {
   say("The barrier seal crumbles.\n");
   write("The barrier seal crumbles.\n");
   destruct(this_object());
}
void time_out() {
// time out disabled this spell lasts till reboot!
}

void extra_long() {
  write(GREY_F+"To the "+QW+" is a barrier seal.\n"+NORM);
}



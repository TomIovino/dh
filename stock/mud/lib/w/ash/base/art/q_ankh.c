#define TO this_object()
#define TP this_player()

#include <mudlib.h>
#include <ansi.h>

inherit ARMOUR;

inherit "inherit/tell_all";

void reset(status arg) {
      set_name("ankh");
      set_alias("ankh");
      set_type("amulet");
      set_ac(6);
      set_bonus_int(2);
      set_bonus_wis(2);
      set_bonus_pri(2);
      set_bonus_foc(2);
      set_weight(10);
      set_value(10000);
      set_ego(175);
      set_info("Lore tells you that if you rub this ankh\n"+
               "and the correct aritifacts are present you\n"+
               "will call an item of power. Lore states that\n"+
               "more then once such combination exists.\n");
      set_short(
       BLUE_F"ancient m"B_BLUE_F"i"NORM CYAN_F"t"B_CYAN_F
       "h"NORM CYAN_F+"r"B_BLUE_F"i"NORM BLUE_F"l ankh"NORM);
      set_long(B_WHITE_F+
       "ancient "NORM+BLUE_F"m"BOLD"i"CYAN_F"t"NORM+CYAN_F+
       "h"B_CYAN_F+"r"+B_BLUE_F"i"NORM+BLUE_F"l"B_WHITE_F" ankh\n"+NORM);



}

init(){
  ::init();
  add_action("rub","rub");
 }

string who;
object player;


void start(){
    player = this_player();
    who = capitalize(this_player()->query_name());
    call_out("finish1",1);
    call_out("finish2",3);
    call_out("finish3",5);
}
void start2(){
    player = this_player();
    who = capitalize(this_player()->query_name());
    call_out("finish1",1);
    call_out("finish2b",3);
    call_out("finish3",5);
}

void do_it(){
  write(
   B_WHITE_F
  "You see a flash of light!\n"
  "The items are gone but something remains in its place.\n"
   );
   start(); 
  return;
}
void do_it2(){
  write(
   B_WHITE_F
  "You see a flash of light!\n"
  "The items are gone but something remains in its place.\n"
   );
   start2(); 
  return;
}

int rub(string it){
  object f,e,a,w, c,o,d,l, A,B;

  if(environment() != this_player()){
     write("Maybe you get the ankh first.\n");
     return 1;
  }

  if(it != "ankh"){ return 0; }

  A = present("stormtamer", TP);
  B = present("stormbringer", TP);
  f = present("sutcher", TP);
  w = present("bladestar", TP);
  a = present("razortooth", TP);
  e = present("smash", TP);
  o = present("starshine", TP);
  c = present("dejavu", TP);
  l = present("lifeguard", TP);
  d = present("boneguard", TP);

  if(A && B){
      destruct(A); 
      destruct(B); 
      move_object(clone_object("w/ash/base/art/q_sword"),
      this_player());
      TP->set_quest("DragonReborn");
      TP->finish_quest("DragonReborn",5);
      do_it2();
      return 1;
     }
  if(f && w && a && e){
      destruct(f); 
      destruct(e); 
      destruct(w); 
      destruct(a); 
      move_object(clone_object("w/ash/base/art/q_wand"),
      this_player());
      TP->set_quest("DragonSlayer");
      TP->finish_quest("DragonSlayer",5);
      do_it();
      return 1;
     }

  if(o && l && d && c){
      destruct(o); 
      destruct(l); 
      destruct(d); 
      destruct(c); 
      move_object(clone_object("w/ash/base/art/q_robe"),
      this_player());
      TP->set_quest("DragonSlayer");
      TP->finish_quest("DragonSlayer",5);
      do_it();
      return 1;
     }

  if(f || w || e ||a || o || c || l || d || A || B){
   write("Something happens but not much.. ?\n");
   return 1;
  }
  return 1;
}


int finish1(){
   tell_all("\n"+GREY_F+
   "You have a vision of the future..\n"+
   "In the distance you hear rain, a storm seems to be moving in.\n"+NORM);
   return 1;
}
int finish2(){
   tell_all("\n"+BOLD+WHITE_F
   "Thunder booms in the forest echoing loudly, "+RED_F+
   "a streak of fire crosses the sky, "+BLUE_F+"\n"+
   "a tidal wave crashes onto the land, "+WHITE_F+
   "a gust of wind gives you an eerie chill, "+YELLOW_F+"\n"+
   "a mountain trembles causing a rockslide.\n"+NORM);
   return 1;
}
int finish2b(){
   tell_all("\n"+BOLD+WHITE_F+
     " Instead of a storm a giant rainbow appears..\n"+
     B_MAGENTA_F+
     " ____________________________________________ \n"+
     B_BLUE_F+
     " ____________________________________________ \n"+
     B_GREEN_F+
     " ____________________________________________ \n"+
     B_RED_F+
     " ____________________________________________ \n"+
     YELLOW_F+
     " ____________________________________________ \n"+
     NORM);
   return 1;
}
int finish3(){
   tell_all(NORM+"\n"+
   " Your mind clears and the vision ceases.\n"+NORM);
return 1;
}


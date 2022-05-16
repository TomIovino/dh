#define TO this_object()
#define TP this_player()

#include <mudlib.h>
#include <ansi.h>

inherit CONTAINER;
inherit "inherit/tell_all";

void reset(status arg) {
      set_name("bag");
      set_short(B_GREEN_F"silk bag with "+
      B_MAGENTA_F+"j"BLUE_F"e"BROWN_F"w"GREY_F"e"RED_F"l"
     +WHITE_F"e"CYAN_F"d "B_GREEN_F"designs");
      set_long(B_GREEN_F"silk bag with "+
      B_MAGENTA_F+"j"BLUE_F"e"BROWN_F"w"GREY_F"e"RED_F"l"
     +WHITE_F"e"CYAN_F"d "B_GREEN_F"designs\n");
     set_info("This bag is highly magicial it can create objects\n"+
              "from other objects placed inside of it. Once the\n"+
              "proper objects are in the bag, simply shake it.\n");
     // someday i could add another quest and use the same bag =)
     set_max_weight(80);
     set_value(10000);
     set_weight(15);
     set_ego(175);
}


do_death(){
    destruct(this_object()); }
death(){
         call_out("do_death",3); 
       }

init(){
  ::init();
  add_action("shake","shake");
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

void do_it(){
  write(
   B_WHITE_F
  "You see a flash of light!\n"
  "The weapons are gone but something remains in its place.\n"
   );
   start(); 
  return;
}

int shake(string it){
  object f,e,a,w;
  int fire,earth,water,air;
  if(environment() != this_player()){
     write("Maybe you get the bag first.\n");
     return 1;
  }
  fire = earth = water = air = 0;
  if(it != "bag"){ return 0; }
  w = present("bladestar", TO);
  if(w){
   if(w->query_name() == "bladestar") water = 1;
  }
  f = present("sutcher", TO);
  if(f){
   if(f->query_name() == "sutcher") fire = 1;
  }
  e = present("smash", TO);
   if(e){
   if(e->query_name() == "smash") earth = 1;
  }
  a = present("razortooth", TO);
   if(a){
   if(a->query_name() == "razortooth") air  = 1;
  }
  if(f && w && a && e){
      destruct(f); 
      destruct(e); 
      destruct(w); 
      destruct(a); 
      move_object(clone_object("w/ash/dragon/fifth/wand"),
      this_object());
      TP->set_quest("DragonSlayer");
      TP->finish_quest("DragonSlayer",5);
      do_it();
      return 1;
     }
  if(!f && !w && !w && !a){
   write("Nothing happens.\n");
   return 1;
  }
  if(f || w || e ||a){
   write("Something happens but not much, maybe its not full?\n");
   return 1;
  }
  return 1;
}


int finish1(){
   tell_all("\n"+GREY_F+
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
int finish3(){
   tell_all(NORM+"\n"+
   who+" has assembled the "BLUE_F"St"BOLD"or"CYAN_F"mb"NORM+CYAN_F+
   "ri"BOLD"ng"NORM+BLUE_F"er, "+NORM+
   "Your mind clears and the vision ceases.\n"+NORM);
return 1;
}


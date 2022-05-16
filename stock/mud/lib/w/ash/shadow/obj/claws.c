#include <ansi.h>
#include <mudlib.h>
inherit WEAPON;

int f,a,e,w;
int open;

void reset(status arg){
    if(arg) return;

    set_name("claws");
    set_short(B_RED_F+"cr"+YELLOW_F+"ys"B_BLUE_F+
    "ta"+B_WHITE_F+"l c"+B_RED_F+"la"+YELLOW_F+"ws"+NORM);
    set_alias("claw");
    set_long(RED_F+
  "A beautiful crystaline gauntlet with four crystal claws.\n"+
  "As you look at the gauntlet you notice four gem slots which\n"+
  "have different shapes and sizes, however the slots are\n"+
  "empty, perhaps you can find what was once inserted in them.\n");
    set_info("These are the claws of elemental force, however the\n"+
             "gems are missing  which give the claws there true\n"+
             "power, find the four gems and 'insert' them in place.\n");
    set_wc(10);
    set_type("tear");
    set_length(10);  
    set_weight(10);
    set_value(500);
}

init(){
 ::init();
 add_action("insert","insert");
}

insert(string what){
   object gem,x;
   x = "/w/ash/shadow/obj/claws2";

   if(what != "ruby"     &&  what != "amber"
   && what != "diamond"  &&  what != "sapphire"
   && what != "triangle" &&  what != "pentagon"
   && what != "pentagon" &&  what != "hexagon"){
            printf("Hmm?\n");
            return 1; }
   if(!gem = present(what, this_player())) {
            printf("You dont seem to have that.\n");
            return 1;  }
   if(gem->query_value() != 250){
           printf("That dosnt seem to fit.\n");
            return 1; }
   write("You insert the "+what+"!\n");
  write("You hear a click.\n");
   if(gem->query_name() == "ruby")         f = 1;
   if(gem->query_name() == "diamond")      a = 1;
   if(gem->query_name() == "amber")        e = 1;
   if(gem->query_name() == "sapphire")     w = 1;
   gem->rto();
   if(f && e && w && a){ open = 1; }
   if(open){
     write(B_GREEN_F"The claws glow with power!\n"NORM);
     say(B_GREEN_F+this_player()->query_alt_name()+
                 " holds glowing claws.\n");
      move_object(clone_object(x),this_player());
      rto();

     return 1;
   }
   if(!f) { write("The triangle slot is still empty.\n");
   }
   if(!w) { write("The square slot is still empty.\n");
   }
   if(!a) { write("The hexagon slot is still empty.\n");
   }
   if(!e) { write("The pentagon slot is still empty.\n");
   }
 return 1;
}



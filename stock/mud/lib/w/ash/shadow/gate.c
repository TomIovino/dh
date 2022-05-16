#include <ansi.h>
#include <mudlib.h>
inherit TREASURE;

int f,w,e,a;
int open;

reset (arg){
    if(arg) return;
    set_weight(1);
    set_value(101);
    set_name("circle");
    set_short("circle");
    set_long("circle");
}

query(){
  return write("f = "+f+"      w = "+w+"     e = "+e+"       a = "+a+"\n");
  }

init(){
 add_action("enter","enter");
 add_action("insert","insert");
}

insert(string what){
   object gem;
   if(what != "ruby"     &&  what != "amber"  
   && what != "diamond"  &&  what != "sapphire"
   && what != "triangle" &&  what != "pentagon"
   && what != "pentagon" &&  what != "hexagon"){
            printf("Hmm?\n");
            return 1; }
   if(!gem = present(what, this_player())) {
            printf("You dont seem to have that.\n");
            return 1;  }
   if(gem->query_value() != 1000){
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
     write(B_GREEN_F"YOU HAVE ACTIVATED THE CIRCLE!\n"NORM);
     say(B_GREEN_F+this_player()->query_alt_name()+
                 " has actived the circle.\n");
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

enter(string what){
  if(!what == "circle"){
    return 0;
  }
  if(!open){
write("You walk through the loop of the circle.. nothing happens.\n");
write("Maybe you need to active the circle somehow.\n");
  return 1;
  }

  write("You feel your self moving.. with out moving..\n");
  write("You feel dizzy and find yourself in another place.\n");
  say(this_player()->query_alt_name()+
      " enters the circle and vanishes.\n");
  this_player()->move_player("X#/w/ash/cloud/enter");
  return 1;

}


query_weight(){ return 1000; }

short(){
return 
B_GREEN_F+
"A circle floats in midair with many "+B_MAGENTA_F+
                                   "glyphs of an unknown origin.\n";
}
long(){
 write(B_GREEN_F+
"You look at the stone ring which hovers here. Its diameter is big\n"+
"enough for a dragon to comfortably walk through it. You look more\n"+
"closely and you notice the following:\n");
    if(f)
    write("A ruby gem is placed in the triangle.\n");
    else
    write("A triangular indentation can be seen.\n");
    if(e)
    write("An amber gem is placed in the pentagon.\n");
    else
    write("A pentagonal indentation can be seen.\n");
    if(w)
    write("A sapphire gem is placed in the square.\n");
    else
    write("A square indentation can be seen.\n");
    if(a)
    write("A diamond is placed in the hexagon.\n");
    else
    write("A hexagonal indentation can be seen.\n");
    if(open)
    write(BLINK+"The gate is open.\n");
    else
    write("This appears to be a portal.\n");
    write(NORM);
  return 1;
}

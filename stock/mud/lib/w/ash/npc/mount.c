#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;

string ownn;
object owno;

string make(string s){
   if(!s) return "noname";
   ownn = s;
   if(!find_player(ownn)) return "nomaster";
   owno = find_player(ownn);
   set_level(owno->query_level());
   set_short(owno->query_color()+owno->query_magic()+" dragon mount");
   set_long(capitalize(owno->query_magic())+" dragon mount.\n");
   return s;
}

reset(arg) {
  ::reset(arg);
  if(arg) return;
  set_name("dragon");
  set_short("dragon mount");
  set_long("dragon mount.\n");
  set_alias("dragon");
  set_race("dragon");
  set_level(1);
  set_gender(1);
  load_chat(10, ({    "*heal",    }));
  set_var("wings",1);
  set_var("wings_time",1000);
  set_var("claws",1);
  set_var("claws_time",1000);
  set_var("sight",1);
  set_var("sight_time",1000);
  set_var("exoskin",1);
  set_var("exoskin_time",1000);
}

init(){
  ::init();
 if(environment(this_player()) == this_object()){
  add_action("dismount","dismount");
  add_action("isee","look");
  add_action("isee","l");
  }
 else{
  add_action("mount","mount");
  }
}

isee(){
     write(environment(this_object())->short()+"\n");
     show_inventory(environment(this_object()));
   return "";
}

mount(){
   if(!this_player()->query_owner() &&
      !this_player()->query_security_level()){
      write("You dont think thats a good idea.\n");
      return 1;
     }
      write("You mount the Dragon.\n");
      say(this_player()->query_alt_name()+" climbs onto "
      +this_player()->query_possessive()+" Dragon Mount.\n");
      move_object(this_player(),this_object());
      say(this_player()->query_alt_name()+" climbs onto "
      +this_player()->query_possessive()+" Dragon Mount.\n");
      return 1;
}

dismount(){
   if(environment(this_player()) != this_object()){
      write("Your not riding a dragon.\n");
      return 1;
      }
      write("You dismount the Dragon.\n");
      say(this_player()->query_alt_name()+" climbs off "
      +this_player()->query_possessive()+" Dragon Mount.\n");
      move_object(this_player(),environment(this_object()));
      say(this_player()->query_alt_name()+" climbs off "
      +this_player()->query_possessive()+" Dragon Mount.\n");
      return 1;
}


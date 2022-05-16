////////////////////////////////////////////////////////////
// TREASURE.C             // DRAGONHEART // ASH.5.17.97   //
////////////////////////////////////////////////////////////

#include <mudlib.h>
inherit BASE;
inherit "inherit/magicstuff";


void reset(status arg) {
  if(arg) return;

  set_weight(1);
  set_value(10);
  set_name("gem");
  set_alias("ruby");
  set_short("Gem");
  set_long("It looks like a small ruby.\n");
  set_extra_info("It doesn't look very valuable.\n");

}


status get(){
  if(this_player()->query_ego() < this_object()->query_ego()){
   write("You try to get "+query_short()+" but it refuses.\n");
   say(this_player()->query_cap_name()+" tries to get "+query_short()+
   " but it refuses.\n");
  return 0;
  }
  return 1;
 }

string query_object_type() { return "treasure"; }

string query_short(){
  return short_desc; }

string short(status wiz){ return query_short(); }


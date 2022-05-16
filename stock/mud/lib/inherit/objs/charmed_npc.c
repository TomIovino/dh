/* basis for a summoned monster */

#include <ansi.h>
#include <mudlib.h>

inherit "inherit/monster";
inherit "inherit/timer";

object master;

object query_master()     { return master; }
void set_master(object m) { master = m; }

 string short(status wiz) {
   return (master ? (string)master->query_name() +"'s " : "") 
        + ::short(wiz);
 }


void heart_beat() {
  object attack, env_master;

  ::heart_beat();
  if(!master) {
    time_out();
    return;
  }
  attack = (object)master->query_primary_attack();
  if(attack == this_object()) {
      tell_room(environment(), "The magic fades away...\n");
      time_out();
      return;
  }
  if(attack) {
    if(attack != query_primary_attack()) {
      add_secondary_attacker(attack);
      set_primary_attack(attack);      
    }
  } else { // !attack
    if(attack = query_primary_attack()) {
      tell_room(environment(),query_name() +" stops fighting.\n");
      stop_fight(attack);
      attack->stop_fight(this_object());
    }
  }
  env_master = environment(master);
  if(!env_master) {
      tell_room(environment(), "The magic fades away...\n");
      time_out();
      return;
  }
  if(env_master != environment()) {
      this_object()->move_player("in search of its master",env_master);
  }
  set_heart_beat(1);
}


status cast_spell(object caster,object target,object prev,int level) {
  object summoned;
  int i;

  master = caster;
  adj_time(30 * 30);
  set_heart_beat(1);
  // 1.summon 2.animated 3.conjured 4.generic
  // tell_room(environment(),"The elemental arrives in a flurry.\n");
  // tell_room(environment(),"The elemental starts to move!\n");
     tell_room(environment(),"The magic appears in a flash of light.\n");
  // tell_room(environment(),"The elemental appears.\n");
      
    if((object)caster->query_attrib("elemental")) {
            write("You cannot control another summoned creature.\n");
            this_player()->unload_spell();
            destruct(this_object());
            return 1;
    } else {
        caster->set_tmp_attrib("elemental",this_object());
    }
  return 1;
}


void time_out() {
//    tell_room(environment(),query_name() +" flees...\n");
//    tell_room(environment(),query_name() +" crumbles to dust!\n");
      tell_room(environment(),query_name() +" disappears...\n");
//    tell_room(environment(),query_name() +" fades away...\n");
  
  ::time_out();
}


status dispel_magic() {  time_out(); }


status monster_died() {
  object corpse;

  remove_call_out("time_out");
  if((corpse = present("corpse", environment()))) destruct(corpse);
  tell_room(environment(),"The corpse disappears.\n");
}    

init() {
  ::init();
}

string query_party() {
    return master ? (string) master->query_party() : 0;
}

int is_elemental() { return 1; }

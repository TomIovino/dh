#include <ansi.h>
/* drain (ash)*/

int time;
int bonus;
int me, you,skill;

status drain(mixed targ, mixed alt_type) {
  int i,x;
  string file;

  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);
  this_player()->load_spell(({
  "target",            targ,
  "name",              "drain",
  "stat",              "intellect",
  "type",              "focus",
  "cost",              25,
  "damage",            25,
  "level",             25,
  "cast time",         2,
  "spell object",      file,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {

  skill = this_player()->query_skill("drain");
  me =  caster->query_level();
  you = target->query_level();

  if(!target->is_living()){ write("Yah, ok..\n"); return 1; }
  if( (random(me)*2) < you){
    write(YELLOW_F+"Your victim resists your drain!\n"+NORM);
     destruct(this_object());
     return 1;
    }

  tell_object(target,"You feel drained!\n");
  if(target != caster) {
    tell_object(caster,"You cast a drain spell on "+ target->query_name() 
                      +".\n");
  }
  say(caster->query_name() +" casts a drain on "+ target->query_name()+".\n"+
      target->query_name() +" looks drained!\n");
  skill = this_player()->query_skill("drain")+5/5;  
  target->adj_mp(-(skill));
  target->adj_hp(-(skill));
  caster->adj_mp((skill));
  caster->adj_hp((skill));
 

  if(!target->query_primary_attacker())
  target->set_primary_attack(caster);
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "drain"
      || str == "external";
}




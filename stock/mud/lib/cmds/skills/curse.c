/* CURSE (ash)*/

int time;
int bonus;
object victim;

status main(mixed targ, mixed alt_type) {
  int i,x;
  string file;

  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);
  this_player()->load_spell(({
  "target",            targ,
  "name",              "curse",
  "type",              "primal",
  "stat",              "wisdom",
  "cost",              20,
  "damage",            50,
  "level",             35,
  "cast time",         3,
  "spell object",      file,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {
  int skill;
  victim = target;

  tell_object(target,"You feel cursed!\n");
  if(target != caster) {
    tell_object(caster,"You cast a curse spell on "+ target->query_name() 
                      +".\n");
  }
  say(caster->query_name() +" casts a curse on "+ target->query_name()+".\n"+
      target->query_name() +" looks cursed!\n");
  skill = this_player()->query_skill("curse");  
  victim->set_var("curse",skill);              
  victim->set_var("curse_time",skill);
  victim->unset_var("bless");
  victim->unset_var("bless_time");
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "curse"
      || str == "external";
}




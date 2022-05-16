/* BLIND (ash)*/

int time;
int bonus;
object victim;

status blind(mixed targ, mixed alt_type) {
  int i,x;
  string file;
  x = this_player()->query_skill("blind");

  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);
  this_player()->load_spell(({
  "target",            targ,
  "name",              "blind",
  "stat",              "wisdom",
  "type",              "primal",
  "cost",              30,
  "damage",            50,
  "level",             50,
  "cast time",         3,
  "spell object",      file,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {
  int skill;
  victim = target;
  tell_object(target,"You are blind!\n");
  if(target != caster) {
    tell_object(caster,"You cast a blind spell on "+ target->query_name() 
                      +".\n");
  }
  say(caster->query_name() +" casts a blind on "+target->query_name()+".\n"+
      target->query_name() +" is blind!\n");
  skill = this_player()->query_skill("blind");  
  victim->set_var("blind",skill);              
  victim->set_var("blind_time",skill);
  victim->unset_var("sight");
  victim->unset_var("sight_time");
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "blind"
      || str == "external";
}





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
  "name",              "weaken",
  "stat",              "wisdom",
  "type",              "primal",
  "cost",              20,
  "damage",            1,
  "level",             50,
  "cast time",         3,
  "spell object",      file,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {
  int skill;
  victim = target;

  tell_object(target,"You feel weakened!\n");
  if(target != caster) {
    tell_object(caster,"You cast a weaken spell on "+ target->query_name() 
                      +".\n");
  }
  say(caster->query_name() +" casts weaken on "+ target->query_name()+".\n"+
      target->query_name() +" looks weakened!\n");
  skill = this_player()->query_skill("weaken");  
  victim->set_var("weaken",skill);              
  victim->set_var("weaken_time",skill);
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "weaken"
      || str == "external";
}




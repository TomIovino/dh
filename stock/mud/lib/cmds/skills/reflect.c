
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
  "name",              "reflect",
  "type",              "focus",
  "stat",              "intellect",
  "cost",              50,
  "damage",            0,
  "level",             25,
  "cast time",         3,
  "spell object",      file,
  "passive", 1,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {
  int skill;
  victim = target;
  tell_object(target,"You become reflective!\n");
  if(target != caster) {
    tell_object(caster,"You cast a reflect spell on "+ 
       target->query_name()+".\n");
  }
  say(caster->query_name() +" casts a reflect on "+
      target->query_name()+".\n"+
      target->query_name() +" becomes reflective!\n");
  skill = this_player()->query_skill("reflect");  
  victim->set_var("reflect",skill);              
  victim->set_var("reflect_time",(skill*3+120));
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "curse"
      || str == "external";
}




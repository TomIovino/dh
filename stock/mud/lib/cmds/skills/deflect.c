
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
  "name",              "deflect",
  "type",              "focus",
  "stat",              "intellect",
  "cost",              50,
  "damage",            0,
  "level",             50,
  "cast time",         3,
  "spell object",      file,
  "passive", 1,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {
  int skill;
  victim = target;
  tell_object(target,"You're deflective!\n");
  if(target != caster) {
    tell_object(caster,"You cast a deflect spell on "+ 
       target->query_name()+".\n");
  }
  say(caster->query_name() +" casts a deflect spell on "+
      target->query_name()+".\n"+
      target->query_name() +" becomes deflective!\n");
  skill = this_player()->query_skill("deflect");  
  victim->set_var("deflect",skill);              
  victim->set_var("deflect_time",(skill*3+120));
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "curse"
      || str == "external";
}




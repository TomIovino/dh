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
  "name",              "diminish",
  "type",              "primal",
  "stat",              "intellect",
  "cost",              35,
  "damage",            0,
  "level",             80,
  "cast time",         3,
  "spell object",      file,
  "passive", 1,
  }));
  return 1;
}



status cast_spell(object caster,object target,object prev,int time) {
  int skill;
  victim = target;
  tell_object(target,"You feel your powers diminish!\n");
  if(target != caster) {
    tell_object(caster,"You cast a diminish spell on "+
    target->query_name()+".\n");
  }
  say(caster->query_name() +" casts a diminish on "+
      target->query_name()+".\n"+
      target->query_name() +" looks frail!\n");
  skill = (this_player()->query_skill("diminish")/5)+1;  
  if(skill > 10) skill = 10;
  victim->set_var("stamina",-skill);              
  victim->set_var("stamina_time",(skill*10)+600);
  caster->set_var("stamina",skill/2);
  caster->set_var("stamina_time",(skill*10)+600);
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "curse"
      || str == "external";
}




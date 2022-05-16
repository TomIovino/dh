
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
  "name",              "fortitude",
  "type",              "primal",
  "stat",              "wisdom",
  "cost",              75,
  "damage",            0,
  "level",             75,
  "cast time",         3,
  "spell object",      file,
  "passive", 1,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {
  int skill;
  victim = target;
  tell_object(target,"You feel fortified!\n");
  if(target != caster) {
    tell_object(caster,"You cast a fortitude spell on "+
    target->query_name()+".\n");
  }
  say(caster->query_name() +" casts a fortitude spell on "+
      target->query_name()+".\n"+
      target->query_name() +" looks fortitified!\n");
  skill = this_player()->query_skill("fortitude");  
  victim->set_var("fortitude",skill);              
  victim->set_var("fortitude_time",(skill*5+60));
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "external";
}




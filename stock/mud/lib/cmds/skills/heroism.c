
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
  "name",              "heroism",
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
  tell_object(target,"You feel heroic!\n");
  if(target != caster) {
    tell_object(caster,"You cast a heroism spell on "+
    target->query_name()+".\n");
  }
  say(caster->query_name() +" casts a heroism spell on "+
      target->query_name()+".\n"+
      target->query_name() +" looks heroic!\n");
  skill = this_player()->query_skill("heroism");  
  victim->set_var("heroism",skill);              
  victim->set_var("heroism_time",(skill*5+60));
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "external";
}




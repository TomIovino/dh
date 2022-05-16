
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
  "name",              "manafuse",
  "type",              "focus",
  "stat",              "intellect",
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
  tell_object(target,"You feel infused!\n");
  if(target != caster) {
    tell_object(caster,"You cast a manafuse spell on "+
    target->query_name()+".\n");
  }
  say(caster->query_name() +" casts a manafuse spell on "+
      target->query_name()+".\n"+
      target->query_name() +" looks infused!\n");
  skill = this_player()->query_skill("manafuse");  
  victim->set_var("manafuse",skill);              
  victim->set_var("manafuse_time",(skill*5+60));
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "external";
}




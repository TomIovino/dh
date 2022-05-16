/* POISON (ash)*/

int time;
int bonus;
object victim;

status bane(mixed targ, mixed alt_type) {
  int i,x;
  string file;

  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);
  this_player()->load_spell(({
  "target",            targ,
  "name",              "bane",
  "type",              "primal",
  "stat",              "wisdom",
  "cost",              80,
  "damage",            1,
  "level",             80,
  "cast time",         3,
  "spell object",      file,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {
  int skill;
  victim = target;
  tell_object(target,"You are poisoned!\n");
  if(target != caster) {
    tell_object(caster,"You cast a bane spell on "+ target->query_name() 
                      +".\n");
  }
  if(target->query_level() > 100){ 
    tell_object(caster,"Your target resists!\n");
    destruct(this_object());
    return 1;
  }
  say(caster->query_name() +" casts bane on "+
      target->query_name()+".\n"+
      target->query_name() +" looks pale!\n");
    log_file("SPELL",this_player()->query_real_name()+" bane'd "+
       target->query_name()+" @ "+ctime()+"\n");

  skill = this_player()->query_skill("bane");  
  victim->set_var("poison",skill*3);              
  victim->set_var("poison_time",skill/2);
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "poison"
      || str == "external";
}




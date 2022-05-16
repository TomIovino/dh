/* POISON (ash)*/

int time;
int bonus;
object victim;

status poison(mixed targ, mixed alt_type) {
  int i,x;
  string file;


  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);
  this_player()->load_spell(({
  "target",            targ,
  "name",              "poison",
  "type",              "primal",
  "stat",              "wisdom",
  "cost",              40,
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
    log_file("SPELL",this_player()->query_real_name()+" poisoned "+
       target->query_name()+" @ "+ctime()+"\n");

  tell_object(target,"You are poisoned!\n");
  if(target != caster) {
    tell_object(caster,"You cast a poison spell on "+ target->query_name() 
                      +".\n");
  }
  say(caster->query_name() +" casts poison on "+
      target->query_name()+".\n"+
      target->query_name() +" is poisoned!\n");
  skill = this_player()->query_skill("poison");  
if(!victim->query_var("poison")
  )victim->set_var("poison",skill);              
if(!victim->query_var("poison_time")
  )victim->set_var("poison_time",skill*5);
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "poison"
      || str == "external";
}




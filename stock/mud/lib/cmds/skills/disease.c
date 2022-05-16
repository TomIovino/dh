/* POISON (ash)*/

int time;
int bonus;
object victim;

status disease(mixed targ, mixed alt_type) {
  int i,x;
  string file;



  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);
  this_player()->load_spell(({
  "target",            targ,
  "name",              "disease",
  "type",              "primal",
  "stat",              "wisdom",
  "cost",              25,
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
 
   log_file("SPELL",this_player()->query_real_name()+" cast disease on "+
       target->query_name()+" @ "+ctime()+"\n");

  tell_object(target,"You are diseased!\n");
  if(target != caster) {
    tell_object(caster,"You cast a disease spell on " 
                   +target->query_name() 
                      +".\n");
  }
  say(caster->query_name() +" casts disease on "+
      target->query_name()+".\n"+
      target->query_name() +" is diseased!\n");
  skill = this_player()->query_skill("disease");  
  victim->set_var("disease",skill);              
  victim->set_var("disease_time",skill*30);
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "poison"
      || str == "external";
}




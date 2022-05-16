/* STAT SPELL (ash)*/

#define STAT "stamina"

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
  "name",              STAT,
  "type",              "focus",
  "stat",              "wisdom",
  "cost",              40,
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
  tell_object(target,"You feel your "+STAT+" increase!\n");
  if(target != caster) {
    tell_object(caster,"You cast a "+STAT+" spell on "+
    target->query_name()+".\n");
  }
  say(caster->query_name() +" casts a "+STAT+" on "+
      target->query_name()+".\n"+
      target->query_name() +" gains "+STAT+"!\n");
  skill = (this_player()->query_skill(STAT)/10)+5;  
  if(skill > 10) skill = 10;
  victim->set_var(STAT,skill);              
  victim->set_var(STAT+"_time",(skill*10)+600);
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "curse"
      || str == "external";
}




/* BLESS (ash)*/

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
  "name",              "bless",
  "stat",              "wisdom",
  "type",              "focus",
  "cost",              30,
  "damage",            0,
  "level",             35,
  "cast time",         3,
  "spell object",      file,
  "passive", 1,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {
  int skill;
  victim = target;
  tell_object(target,"You feel blessed!\n");
  if(target != caster) {
    tell_object(caster,"You cast a bless spell on "+ target->query_name() 
                      +".\n");
  }
  say(caster->query_name() +" casts a bless on "+
      target->query_name()+".\n"+
      target->query_name() +" looks blessed!\n");
  skill = this_player()->query_skill("bless");  
  victim->set_var("bless",skill);              
  victim->set_var("bless_time",(skill*3+120));
  victim->unset_var("curse");
  victim->unset_var("curse_time");

  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "bless"
      || str == "external";
}




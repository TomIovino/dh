#define TP this_player()
#define TPQN this_player()->query_name()

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
  "name",              "awareness",
  "type",              "focus",
  "stat",              "intellect",
  "cost",              25,
  "damage",            0,
  "level",             25,
  "cast time",         4,
  "spell object",      file,
  "passive",           1,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {
  int skill;
  victim = target;
  tell_object(target,"Your eyes radiate!\n");
  if(target != caster) {
    tell_object(caster,"You cast awareness on "+ target->query_name() 
                      +".\n");
  }
  skill = caster->query_skill("awareness");
  say(caster->query_name() +" casts awareness on "+
      target->query_name()+".\n"+
      target->query_name() +"'s eyes radiate!\n");
      target->set_var("awareness",skill);
      target->set_var("awareness_time",skill*10);

     
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "external";
}




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
  "name",              "sight",
  "type",              "focus",
  "stat",              "intellect",
  "cost",              25,
  "damage",            0,
  "level",             25,
  "cast time",         3,
  "spell object",      file,
  "passive", 1,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {
  int skill;
  victim = target;
  tell_object(target,"Your eyes glow!\n");
  if(target != caster) {
    tell_object(caster,"You cast sight on "+ target->query_name() 
                      +".\n");
  }
  say(caster->query_name() +" casts sight on "+
      target->query_name()+".\n"+
      target->query_name() +"'s eyes glow!\n");
   skill = this_player()->query_skill("sight");
  victim->set_var("sight",skill);
  victim->set_var("sight_time",600);
  victim->unset_var("blind");
  victim->unset_var("blind_time");

  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "external";
}




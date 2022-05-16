#include <ansi.h>

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
  "name",              "defender",
  "type",              "focus",
  "stat",              "intellect",
  "cost",              50,
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
  victim = this_player();
  tell_object(target,"You summon a defender!\n");
  say(B_BLUE_F+caster->query_name() +" casts a defender spell!\n");
  skill = this_player()->query_skill("defender");  
  victim->set_var("defender",skill);              
  victim->set_var("defender_time",(skill*3+120));
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "garbage";
}




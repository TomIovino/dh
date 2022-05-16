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
  "name",              "avenger",
  "type",              "focus",
  "stat",              "intellect",
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
  victim = this_player();
  tell_object(target,"You summon an avenger!\n");
  say(B_BLUE_F+caster->query_name() +" summons an avenger!\n");
  skill = this_player()->query_skill("avenger");  
  victim->set_var("avenger",skill);              
  victim->set_var("avenger_time",(skill*3+120));
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "garbage";
}




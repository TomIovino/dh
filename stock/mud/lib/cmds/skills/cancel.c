#include <ansi.h>

int time;
int bonus;
object victim;

status cancel(mixed targ, mixed alt_type) {
  int i,x;
  string file;

  if(!targ){ write("cancel <what>\n"); return 1; }


  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);
  this_player()->load_spell(({
  "target",            targ,
  "name",              "cancel",
  "stat",              "intellect",
  "type",              "focus",
  "cost",              60,
  "damage",            1,
  "level",             60,
  "cast time",         1,
  "spell object",      file,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {
  int me,you,bonus,skill,l;
  bonus = 0;
  skill = this_player()->query_skill("cancel");  
  me =  caster->query_level();
  you = target->query_level();

  if(you > 100) you = you*5; // level 101+ are virtual immune to dispel.

  if(me+random(me) < you+random(you)){
    write(YELLOW_F+"Your victim resists your cancel!\n"+NORM);
      destruct(this_object());
     return 1;
    }

  tell_object(target,"You have been canceled!\n");
  if(target != caster) {
    tell_object(caster,"You begin to cancel the magic around "+
             target->query_name() +".\n");
  }
  say(caster->query_name() +" cancel "+ target->query_name()+".\n"+
      target->query_name() +" looks canceled!\n");
      target->unset_var("avenger");
      target->unset_var("defender");
      target->unset_var("reflect");
      target->unset_var("absorb");
      destruct(this_object());
  return 1; 
} 

int query_no_save(){ return 1; }

status id(string str) {
  return str == "spell"
      || str == "curse"
      || str == "external";
}




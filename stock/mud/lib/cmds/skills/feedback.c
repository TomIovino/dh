#include <ansi.h>
#define X GREY_F

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
  "name",              "feedback",
  "type",              "focus",   
  "stat",              "intellect",
  "cost",              90,
  "damage",            0,
  "level",             90,          
  "cast time",         1,
  "spell object",      file,
  "passive", 1,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {
  int skill;
  victim = target;

  if(target == caster) {
    tell_object(caster,caster->query_color()+X+"You cast a "+
    caster->query_magic()+
    " aura of feedback on "+ 
    "yourself.\n"+NORM); 
  }
  if(target != caster) {
    tell_object(caster,caster->query_color()+X+"You cast a "+
    caster->query_magic()+
    " aura of feedback on "+ 
    target->query_name()+".\n"+NORM); 
  }
  say(caster->query_color()+X+caster->query_name() +" casts a "+
     caster->query_magic()+
     " aura of feedback on "+
      target->query_name()+".\n"+
      target->query_name() +" has an aura of feedback!\n"+NORM);
  tell_object(target,X+"You have an aura of feedback!\n"+NORM);
  skill = this_player()->query_skill("feedback");  
  victim->set_var("feedback",skill);              
  victim->set_var("feedback_time",(skill/10)+5);
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "external";
}




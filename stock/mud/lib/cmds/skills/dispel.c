#include <ansi.h>
/* dispel (ash)*/

int time;
int bonus;
object victim;

status dispel(mixed targ, mixed alt_type) {
  int i,x;
  string file;

  if(!targ){ write("dispel <who|what>\n"); return 1; }


  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);
  this_player()->load_spell(({
  "target",            targ,
  "name",              "dispel",
  "stat",              "wisdom",
  "type",              "focus",
  "cost",              50,
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
  skill = this_player()->query_skill("dispel");  
  me =  caster->query_level();
  you = target->query_level();

  if(you > 100) you = you*5; // level 101+ are virtual immune to dispel.
  if(me+random(me) < you+random(you)){
    write(YELLOW_F+"Your victim resists your dispel!\n"+NORM);
      destruct(this_object());
     return 1;
    }

  tell_object(target,"You have been dispelled!\n");
  if(target != caster) {
    tell_object(caster,"You begin to dispel the magic around "+
             target->query_name() +".\n");
  }
  say(caster->query_name() +" dispels "+ target->query_name()+".\n"+
      target->query_name() +" looks dispelled!\n");
      target->unset_var("bless");
      destruct(this_object());
  return 1; 
} 

int query_no_save(){ return 1; }

status id(string str) {
  return str == "spell"
      || str == "curse"
      || str == "external";
}




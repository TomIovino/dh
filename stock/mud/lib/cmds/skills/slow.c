#include <ansi.h>

status slow(string targ, mixed alt_type) {

  int i,x;
  string file;
  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);


  this_player()->load_spell(({
  "target",            targ,
  "name",              "slow",
  "stat",              "wisdom",
  "type",              "wisdom",
  "level",             50,
  "cost",              50,
  "damage",            1,
  "immune",	       "hold",
  "level",             75,
  "cast time",         2,
  "aggressive", 1,
  "spell object",      file,
  }));

  return 1;
}

int query_no_save(){ return 1; } /* dont wanna save bugged objs */

status cast_spell(object caster,object target,object prev,int dmg) {
  int me,you,bonus,skill;

  bonus = 0;
  skill = this_player()->query_skill("slow");
  me =  caster->query_level();
  you = target->query_level();

  dmg = (skill+bonus)/10; 
  dmg = random(dmg);

  if(you > 99) you = you*2;

  if(me+random(me) < you+random(you)){
      write(YELLOW_F+"Your victim resists your slow!\n"+NORM);
      destruct(this_object());
      return 1;
   }

  tell_object(caster,"You see "+(string)target->query_name()+
               " stagger as they are slowed!\n");
  tell_object(target,"You feel a slowing effect!\n");
  say((string)target->query_name()+" suddenly slows.\n",
       target);

  if(dmg > 5) dmg = 5;
  if(dmg < 2) dmg = 2;
  target->unset_var("frenzy");
  target->set_var("slow",dmg);
  target->set_var("slow_time",skill*10);
  destruct(this_object());
  return 1;
}


status id(string str) {
  return str == "spell"
      || str == "Hold";
}




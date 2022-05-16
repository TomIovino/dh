#include <ansi.h>

int time;
int bonus;
object victim;

status cripple(mixed targ, mixed alt_type) {
  int i,x;
  string file;

  if(!targ){ write("cripple <who>\n"); return 1; }


  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);
  this_player()->load_spell(({
  "target",            targ,
  "name",              "cripple",
  "stat",              "intellect",
  "type",              "intellect",
  "cost",              75,
  "damage",            1,
  "level",             75,
  "cast time",         1,
  "spell object",      file,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {
  int me,you,bonus,skill,l;
  bonus = 0;
  skill = this_player()->query_skill("cripple");  
  me =  caster->query_level();
  you = target->query_level();

   log_file("SPELL",this_player()->query_real_name()+" cripple "+
       target->query_name()+" @ "+ctime()+"\n");


  if(you > 100) you = you*5; // level 101+ are virtual immune to dispel.
  if(me+random(me) < you+random(you)){
    write(YELLOW_F+"Your victim resists your cripple!\n"+NORM);
      destruct(this_object());
     return 1;
    }
  if(target->query_var("heroism")){
    write(YELLOW_F+"Your victim resists your cripple!\n"+NORM);
      destruct(this_object());
     return 1;
    }
  tell_object(target,"You have been crippled!\n");
  if(target != caster) {
    tell_object(caster,"You begin to cast cripple on "+
             target->query_name() +".\n");
  }
  say(caster->query_name() +" crippled "+ target->query_name()+".\n"+
      target->query_name() +" looks crippled!\n");
      target->unset_var("strength");
      target->unset_var("dexterity");
      target->unset_var("agility");
      target->unset_var("stamina");
      target->set_var("stun",100);
      target->set_var("stun_time",3);
      destruct(this_object());
  return 1; 
} 

int query_no_save(){ return 1; }

status id(string str) {
  return str == "spell"
      || str == "curse"
      || str == "external";
}




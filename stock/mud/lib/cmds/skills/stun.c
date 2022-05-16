#include <ansi.h>
// *Stun (ash)

status stun(string targ, mixed alt_type) {

  int i,x;
  string file;
  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);



  this_player()->load_spell(({
  "target",            targ,
  "name",              "stun",
  "stat",              "wisdom",
  "type",              "primal",
  "level",             50,
  "cost",              50,
  "damage",            1,
  "immune",	       "hold",
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
  skill = this_player()->query_skill("stun");
  me =  caster->query_level();
  you = target->query_level();

  dmg = (skill+bonus)/10; 
  dmg = random(dmg);

  if(you > 101) you = you*5;

    log_file("SPELL",this_player()->query_real_name()+" stun on "+
       target->query_name()+" @ "+ctime()+"\n");

  if(me+random(me) < you+random(you)){
      write(YELLOW_F+"Your victim resists your stun!\n"+NORM);
      destruct(this_object());
      return 1;
   }

  if(target->query_var("stun"))
  {
    tell_object(caster,"Your prey is already stunned!");
    destruct(this_object());
    return 1;
  }
 
  tell_object(caster,"You see "+(string)target->query_name()+
               " stagger as they are stunned!\n");
  tell_object(target,"You reel from the spell's stunning effect!\n");
  say((string)target->query_name()+" suddenly staggers.\n",
       target);

  if(dmg > 5) dmg = 5;
  if(dmg < 2) dmg = 2;
  target->set_var("stun",dmg);
  target->set_var("stun_time",dmg);
  destruct(this_object());
  return 1;
}


status id(string str) {
  return str == "spell"
      || str == "Hold";
}




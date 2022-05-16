#define TP this_player()
#define TPQN this_player()->query_name()

int time;
int bonus;
object victim;

string *query_cmd_aliases() { return ({"regen", }); }

status main(mixed targ, mixed alt_type) {
  int i,x;
  string file;

  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);
  this_player()->load_spell(({
  "target",            targ,
  "name",              "regenerate",
  "stat",              "wisdom",
  "type",              "primal",
  "cost",              30,
  "damage",            0,
  "level",             30,
  "cast time",         3,
  "spell object",      file,
  "passive", 1,
  }));
  return 1;
}

status cast_spell(object caster,object target,object prev,int time) {
  int skill;
  victim = target;
  tell_object(target,"You begin to regenerate!\n");
  if(target != caster) {
    tell_object(caster,"You cast a regenerate spell on "+target->query_name() 
                      +".\n");
  }
  say(caster->query_name() +" casts regenerate on "+
      target->query_name()+".\n"+
      target->query_name() +" begins to regenerate!\n");
      target->set_var("regenerate",1);
      target->set_var("regenerate_time",600);
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "external";
}




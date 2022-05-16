#define TP this_player()
#define TPQN this_player()->query_name()

int time;
int bonus;
object victim;

//string *query_cmd_aliases() { return ({"n", }); }

status main(mixed targ, mixed alt_type) {
  int i,x;
  string file;
  object caster;
  caster = this_player();

  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);
  this_player()->load_spell(({
  "target",            targ,
  "name",              "claws",
  "type",              "primal",
  "stat",              "intellect",
  "cost",              25,
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
  victim = target;
  tell_object(target,"Sharp claws extend from your hands!\n");
  if(target != caster) {
    tell_object(caster,"You cast a claws spell on "+ target->query_name() 
                      +".\n");
  }
  say(caster->query_name() +" casts a claws on "+
      target->query_name()+".\n"+
      target->query_name() +" sprouts magic claws!\n");
  victim->set_var("claws",1);
  victim->set_var("claws_time",600);
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "external";
}




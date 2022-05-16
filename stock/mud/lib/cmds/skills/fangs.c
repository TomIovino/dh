#define TP this_player()
#define TPQN this_player()->query_name()

int time;
int bonus;
object victim;

//string *query_cmd_aliases() { return ({"n", }); }

status main(mixed targ, mixed alt_type) {
  int i,x;
  string file;
/*
  if(!targ)
      if(TP->query_race() == "dragonkin" ||
         TP->query_race() == "suntiger" ||
         TP->query_race() == "shadow" ||
         TP->query_race() == "icewolf" ||
         TP->query_race() == "undead"){
         write("Your fangs extend out of your gums.\n");
         say(TPQN+" extends "+TP->query_possessive()+
                  " fangs out of "+TP->query_possessive()+" gums.\n");
         this_player()->set_var("fangs",1);
         this_player()->set_var("fangs_time",3600);
         return 1;
  }
*/

  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);
  this_player()->load_spell(({
  "target",            targ,
  "name",              "fangs",
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
  tell_object(target,"A pair of sharp fangs extend from your gums!\n");
  if(target != caster) {
    tell_object(caster,"You cast a fangs spell on "+ target->query_name() 
                      +".\n");
  }
  say(caster->query_name() +" casts a fangs on "+
      target->query_name()+".\n"+
      target->query_name() +" sprouts magic fangs!\n");
  victim->set_var("fangs",1);
  victim->set_var("fangs_time",600);
  destruct(this_object());
  return 1; 
} 


status id(string str) {
  return str == "spell"
      || str == "external";
}




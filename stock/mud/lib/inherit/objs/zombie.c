/* animated skeleton object */

inherit "inherit/objs/summoned";

void reset(status arg) {
    ::reset(arg);
    if (arg) return;
set_name("unnamed");
    set_alt_name("zombie");
    set_long("It is a corpse of rotting flesh. The flesh is putrid.\n"+
             "Its power of locamotion "+
             "must come from some unnatural evil magic.\n");
    set_smell("The skeleton smells really bad.\n");
    set_msgin("follows its master");
    set_msgout("goes after its master");
    set_alias("animated");
    set_level(30);
    set_no_exp(1);
    set_enchanted(1);
}


status cast_spell(object caster,object target,object prev,int level) {
  int type, number;
  object summoned;
  int i;

  if(environment(caster)->query_no_summon()) {
    write("Something prevents you summoning.\n");
    destruct(this_object());
    return 1;
  }

  for(i = 1; summoned = present("animated "+ i, environment(caster)); i++) {
    if((object)summoned->query_master() == caster) {
      write("You cannot control another animated creature.\n");
      destruct(this_object());
      return 1;
    } 
  }
  if((summoned = present("corpse", caster)) && summoned->query_name()) {
    set_name((string)summoned->query_name());
    set_short("Zombie of "+ (string)summoned->query_name());
  }
  else {
    set_name("zombie");
    set_short("Zombie");
  }
  clone_object("obj/shadows/follow")->follow(caster,this_object());
  return ::cast_spell(caster,target,prev,level);
}



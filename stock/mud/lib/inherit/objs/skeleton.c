/* animated skeleton object */

inherit "/inherit/objs/summoned";

void reset(status arg) {
    ::reset(arg);
    if (arg) return;
set_name("unnamed");
    set_alt_name("skeleton");
    set_long("It is a sketeton of white bone. The bones are held\n"+
             "together with rotting sinues. It power of locamotion\n"+
             "must come from some unnatural evil magic.\n");
    set_smell("The skeleton smells really bad.\n");
    set_msgin("follows its master");
    set_msgout("goes after its master");
    set_alias("animated");
    set_enchanted(1);
    set_level(15);
    set_no_exp(1);
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
  if((summoned = present("skeleton", caster)) && summoned->query_name()) {
    set_name((string)summoned->query_name());
    set_short("Skeleton of "+ (string)summoned->query_name());
  }
  else {
    set_name("skeleton");
    set_short("Skeleton");
  }
  clone_object("obj/shadows/follow")->follow(caster,this_object());
  return ::cast_spell(caster,target,prev,level);
}



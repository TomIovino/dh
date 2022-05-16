#include <ansi.h>


status barrier(string targ, mixed alt_type) {
  int level, spell_level,i;
  string type;
   string check;
  object summoned,caster;
  if(targ != "seal north" &&
     targ != "seal south" &&
     targ != "seal west"  &&
     targ != "seal east"){
     notify_fail("barrier seal <north/east/south/west>?\n");
    return 0;
  }

    targ = subst(targ," ","_");      
    caster = this_player();
    for(i = 1; summoned = present("seal", environment(caster));i++) {
    if((object)summoned->query_master() == caster) {
      write("You can only have one seal here.\n");
      this_player()->unload_spell();
      destruct(this_object());
      return 1;
    }
  }


  this_player()->load_spell(({
  "target",            environment(this_player()),
  "name",              "barrier seal",
        "stat",        "intellect",
        "type",        "focus",
  "cost",              50,
  "damage",            level,
  "level",             50,
  "cast time",         3,
  "casting msg",       GREY_F+"You call forth arcane powers.\n"+NORM,
  "spell object",      "inherit/objs/"+targ,
  "passive",
  }));
  return 1;
}




// holy ground 97 [ash]

#define TP this_player()
#include <ansi.h>
inherit "inherit/timer";

status main(string spell_type, mixed alt_type) {
  int i, time;
  string file;

  if(spell_type != "seal") {
    notify_fail("magic seal?\n");
    return 1;
  }


  time = TP->query_skill("magic seal")+TP->query_stat("intellect");
  file = file_name(this_object());
  sscanf(file, "%s#%d", file, i);

  this_player()->load_spell(({
  "target",            environment(this_player()),
  "name",              "magic seal",
        "stat",        "intellect",
        "type",        "focus",
  "cost",              75,
  "level",             75,
  "cast time",         3,
  "spell object",      file,
  "damage",            time,
  "casting msg",       "You feel magic leave the area.\n",
  "casting msg room",  "You feel an absence of magic....\n",
  "passive",
  }));
  return 1;
}


/*******************************************************************/
/* called when this is cloned to target */


status cast_spell(object caster,object target,object prev,int dmg) {
  tell_room(target, "The air shimmers about you.\n");
  adj_time(dmg/2);
  return 1;
}  

void time_out() {
  tell_object(environment(),"The holy protection fades...\n");
  ::time_out();
}

void extra_long() {
  write(BOLD+WHITE_F+"The seal of magic is here.\n");
}

status id(string str) {
  return str == "spell"
      || str == "Nulmagic"
      || str == "seal"
      || str == "privacy seal"
      || str == "holy ground";
}



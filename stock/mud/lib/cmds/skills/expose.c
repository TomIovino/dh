
#include <ansi.h>
#define TPDCC (string)caster->query_color()
#define CAST  (string)caster->query_name()
#define TARG  (string)target->query_name()

status expose(string targ, mixed alt_type) {
  string file;
  int i,x;
  x = this_player()->query_skill("expose");
  file = file_name(this_object());
  sscanf(file,"%s#%d",file,i);
   if(!this_player()->query_wp())
   {
    write("You must have a wp to use this skill.\n");
    return 1;
   }


  
  this_player()->load_spell(({
  "target",            targ,
  "name",              "expose",
  "level",             25,
  "stat",              "wisdom",
  "type",              "primal",
  "cost",              50,
  "cast time",         1,
  "spell object",      file,
  "passive",      1,
  "area",
  "casting msg",  ({
                    "You begin to recite a chant.\n",
                    "You call on arcanius energia.\n",
                  }),
  }));
  return 1;
}


/*********************************************************************/
/* when cloned to player */


status cast_spell(object caster,object target,object prev,int dmg) {
      int wis_mod;
      string TGOD;
      write(TPDCC+"You call on the powers of "
          +TGOD+" arcanius energia.\n");
      if(target->query_security_level()){
        tell_object(target, "Someone cast expose.\n"NORM);
        destruct(this_object());
        return 1;
      }
      if(target != caster)
        tell_object(target, "You feel exposed!\n"NORM);
        target->set_invis(0);
        target->set_hiding(0);
        target->set_sneaking(0);
        destruct(this_object());
        return 1;
}
int query_no_save(){ return 1; }


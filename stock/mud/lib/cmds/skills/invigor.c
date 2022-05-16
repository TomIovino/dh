/* prayer (ash) */

#include <ansi.h>
#define TPDCC (string)caster->query_color()
#define CAST  (string)caster->query_name()
#define TARG  (string)target->query_name()

status invigor(string targ, mixed alt_type) {
  string file;
  int i,x;
  x = this_player()->query_skill("invigor");
  file = file_name(this_object());
  sscanf(file,"%s#%d",file,i);
  
  this_player()->load_spell(({
  "target",            targ,
  "name",              "invigor",
  "level",             25,
  "stat",              "wisdom",
  "type",              "primal",
  "cost",              50,
  "cast time",         2,
  "spell object",      file,
  "passive",
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
      TGOD =     (string)caster->query_alignment()+" spirits";
      wis_mod =  (int)this_player()->query_stat("wisdom");
      wis_mod += (int)this_player()->query_skill("invigor");
      wis_mod =  (wis_mod/4)+random(wis_mod/4);
      target->adj_mp(wis_mod);
      write(TPDCC+"You call on the powers of "
          +TGOD+" arcanius energia.\n");
      if(target != caster)
      tell_object(target,TPDCC+caster->query_cap_name()+ 
         " recites a "+TGOD+" chant.\n");
     tell_object(target, "You feel your vitality return.\n"NORM);
     destruct(this_object());
  return 1;
}


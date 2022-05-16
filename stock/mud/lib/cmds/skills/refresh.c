/* refresh */

#include <ansi.h>
#define CAST (string)caster->query_name()
#define TARG (string)target->query_name()

status main(string targ, mixed alt_type) {
  string file;
  int i;

  file = file_name(this_object());
  sscanf(file,"%s#%d",file,i);
  
  this_player()->load_spell(({
  "target",            targ,
  "name",              "refresh",
  "level",             25,
  "stat",              "wisdom",
  "type",              "focus",
  "cost",              30,
  "damage",            -1,
  "cast time",         2,
  "spell object",      file,
  "passive",           1,
   "casting msg",  ({
                    "You feel an energy spread throughout your body.\n",
                    "Powerful energies swirl about you.\n",
                  }),
  }));
  return 1;
}


/*********************************************************************/
/* when cloned to player */


status cast_spell(object caster,object target,object prev,int dmg) {
      int wis_mod;
      wis_mod =  (int)this_player()->query_stat("wisdom");
      wis_mod += (int)this_player()->query_skill("refresh");
      wis_mod =  (wis_mod/3)+random(wis_mod/3);
      target->adj_mp(wis_mod);
  if(target != caster) {
    write(caster->query_color()+"You call on the powers of "+
          caster->query_alignment()+" to refresh "+TARG+"\n"+NORM);
      say(caster->query_color()+CAST+" calls on the powers of "+
          caster->query_alignment()+" to refresh "+TARG+"\n"+NORM);
    tell_object(target, caster->query_color()+CAST +" lays "+
     caster->query_possessive()+" hands upon you.\nYou feel better!\n"NORM);  
  }  
  else {  
 write(caster->query_color()+"You call on the powers of "+
       caster->query_alignment()+" to refresh yourself.\n"+NORM);
   say(caster->query_color()+CAST+" calls on the powers of "+
      caster->query_alignment()+" to refresh "
      + caster->query_objective()+"self.\n"+NORM);
  }  
  destruct(this_object());
  return 1;
}

  query_no_save(){ return 1; }


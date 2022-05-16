/* prayer (ash) */

#include <ansi.h>
#define TPDCC (string)caster->query_color()
#define CAST  (string)caster->query_name()
#define TARG  (string)target->query_name()

status prayer(string targ, mixed alt_type) {
  string file;
  int i,x;
  x = this_player()->query_skill("prayer");
  file = file_name(this_object());
  sscanf(file,"%s#%d",file,i);
  
  this_player()->load_spell(({
  "target",            targ,
  "name",              "prayer",
  "level",             50,
  "stat",              "wisdom",
  "type",              "primal",
  "cost",              50,
  "cast time",         2,
  "spell object",      file,
  "passive",
  "area",
  "casting msg",  ({
                    "You begin to recite your prayer.\n",
                    "You call on the greater powers to hear you.\n",
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
      wis_mod += (int)this_player()->query_skill("prayer");
      wis_mod =  (wis_mod/4)+random(wis_mod/4);
      target->adj_hp(wis_mod);
   if(target->query_alignment() == "good"){
      write(YELLOW_F+"A beam of holy light flashes in the room\n");
      target->adj_hp(wis_mod);
   }      
      write(TPDCC+"You call on the powers of "
          +TGOD+" with a simple prayer.\n");
      if(target != caster)
      tell_object(target,TPDCC+caster->query_cap_name()+ 
         " recites a "+TGOD+" prayer.\n");
     tell_object(target, "You feel better.\n"NORM);
     destruct(this_object());
  return 1;
}



#include <ansi.h>
#define CAST (string)caster->query_name()
#define TARG (string)target->query_name()

status mend(string targ, mixed alt_type) {
  string file;
  int i;

  file = file_name(this_object());
  sscanf(file,"%s#%d",file,i);
  
  this_player()->load_spell(({
  "target",            targ,
  "name",              "mend",
  "level",             25,
  "stat",              "wisdom",
  "type",              "focus",
  "cost",              25,
  "damage", -1,
  "cast time",         4,
  "spell object",      file,
  "passive",
   "casting msg",  ({
                    "You feel a warmth spread throughout your body.\n",
                    "Powerful mending energies swirl about you.\n",
                  }),
  }));
  return 1;
}


/*********************************************************************/
/* when cloned to player */


status cast_spell(object caster,object target,object prev,int dmg) {
      int wis_mod;
      wis_mod =  caster->query_stat("wisdom");
      wis_mod += caster->query_skill("mend");
      wis_mod =  wis_mod+random(wis_mod);
      target->adj_hp(wis_mod/2);

  if(target != caster) {
    write(B_WHITE_F+"Your hands glow with a soft light.\n"+NORM);
    write(YELLOW_F+"You call on the powers of "+
          caster->query_alignment()+" to mend "+TARG+"\n"+NORM);
      say(B_WHITE_F+caster->query_color()+CAST 
         +" hands glow with a soft light.\n"+NORM);
      say(YELLOW_F+CAST+" calls on the powers of "+
          caster->query_alignment()+" to mend "+TARG+"\n"+NORM);
    tell_object(target, CAST+" lays "+
     caster->query_possessive()+" hands upon you.\nYou feel better!\n"NORM);  

  }  
  else {  
   write(B_WHITE_F+"Your hands glow with a soft light.\n"+NORM);
   write("You call on the powers of "+
       caster->query_alignment()+" to mend yourself.\n"+NORM);
   say(B_WHITE_F+caster->query_color()+CAST 
         +" hands glow with a soft light.\n"+NORM);
   say(CAST+" calls on the powers of "+
      caster->query_alignment()+" to mend "
      + caster->query_objective()+"self.\n"+NORM);
  }  
  destruct(this_object());
  return 1;
}

  query_no_save(){ return 1; }


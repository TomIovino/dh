/* HEAL */
string *query_cmd_aliases() { return
({"h", }); }


#include <ansi.h>
#define CAST (string)caster->query_name()
#define TARG (string)target->query_name()

status heal(string targ, mixed alt_type) {
  string file;
  int i;

  file = file_name(this_object());
  sscanf(file,"%s#%d",file,i);
  
  this_player()->load_spell(({
  "target",            targ,
  "name",              "heal",
  "level",             50,
  "stat",              "wisdom",
  "type",              "focus",
  "cost",              50,
  "damage", -1,
  "cast time",         2,
  "spell object",      file,
  "passive",
   "casting msg",  ({
                    "You feel a warmth spread throughout your body.\n",
                    "Powerful healing energies swirl about you.\n",
                  }),
  }));
  return 1;
}


/*********************************************************************/
/* when cloned to player */


status cast_spell(object caster,object target,object prev,int dmg) {
      int wis_mod;
      wis_mod =  caster->query_stat("wisdom");
      wis_mod += caster->query_skill("heal");
      wis_mod =  wis_mod+random(wis_mod);
      target->adj_hp(wis_mod)+25;

  if(target != caster) {
    write(B_WHITE_F+"Your hands glow with a soft light.\n"+NORM);
    write(YELLOW_F+"You call on the powers of "+
          caster->query_alignment()+" to heal "+TARG+"\n"+NORM);
      say(B_WHITE_F+caster->query_color()+CAST 
         +" hands glow with a soft light.\n"+NORM);
      say(YELLOW_F+CAST+" calls on the powers of "+
          caster->query_alignment()+" to heal "+TARG+"\n"+NORM);
    tell_object(target, CAST+" lays "+
     caster->query_possessive()+" hands upon you.\nYou feel better!\n"NORM);  

  }  
  else {  
   write(B_WHITE_F+"Your hands glow with a soft light.\n"+NORM);
   write("You call on the powers of "+
       caster->query_alignment()+" to heal yourself.\n"+NORM);
   say(B_WHITE_F+caster->query_color()+CAST 
         +" hands glow with a soft light.\n"+NORM);
   say(CAST+" calls on the powers of "+
      caster->query_alignment()+" to heal "
      + caster->query_objective()+"self.\n"+NORM);
  }  
  destruct(this_object());
  return 1;
}

  query_no_save(){ return 1; }


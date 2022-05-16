////////////////////////////////////////////////////////////
// STORM.c      //       DRAGONHEART       // ASH.8.01.98 //
////////////////////////////////////////////////////////////



#define TP this_player()
 
#define NAME "@@query_name:$this_player()$@@"  /* gives greater flexibilty */
#define TARG "@@query_name:$this_object()$@@"
#include <mudlib.h>
#include <ansi.h>
 


status main(string targ, mixed alt_type){
 string file;
 int stat, skill, dam, level,i;
 string ELEM;
  ELEM  = this_player()->query_magic();
  level = this_player()->query_level();
  stat  = this_player()->query_stat("intellect");
  skill = this_player()->query_skill("storm");

 dam = ( random(stat+skill+level)+stat+skill+level ) / 3;

  this_player()->load_spell(({
  "spell object",       __FILE__,
  "target",            targ,
  "name",              "storm",
        "stat",   "intellect",
        "type",   "primal",
  "cost",              60,
  "level",             60,
  "damage",            dam,
  "cast time",         3,
  "area",
  "casting msg", ({
    GREY_F+"You focus on the forces of "+ELEM+".\n",
        }),
  "msg target",
RED_F+BLINK+
"A "BOLD+GREEN_F"s"+BLUE_F+"t"RED_F"o"YELLOW_F"r"MAGENTA_F+"m"NORM+RED_F+BLINK+
" of "+ELEM+" radiates from "+NAME+"'s mind destroying you.\n"+NORM,
  "msg room",
RED_F+BLINK+
"A "BOLD+GREEN_F"s"+BLUE_F+"t"RED_F"o"YELLOW_F"r"MAGENTA_F+"m"NORM+RED_F+BLINK+
" of  "+ELEM+" radiates from "+NAME+"'s mind destroying "+TARG+".\n"+NORM,
  "msg caster",
RED_F+BLINK+
"A "BOLD+GREEN_F"s"+BLUE_F+"t"RED_F"o"YELLOW_F"r"MAGENTA_F+"m"NORM+RED_F+BLINK+
" of "+ELEM+" radiates from your mind destroying "+ TARG +".\n"+NORM,
  }));
  return 1;
}
 


/********************************************************************/
/* when cloned to target */
 
status cast_spell(object caster,object target,object prev,int dmg) {
   int cast_halted;
  
  if(target == caster) 
    cast_halted++;
  
  destruct(this_object());
  return cast_halted;
}



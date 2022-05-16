#include <ansi.h>
#include <mudlib.h>
#include <bar.h>
#define TP find_player(arg)

string answer(int x){ 
  if(x) return  (string)B_GREEN_F+"yes"+YELLOW_F;
  if(!x) return (string)B_RED_F+"no "+YELLOW_F;
}


string *query_cmd_aliases() { return 
({ "aff", "affs", "affect","eff","effect","effects", });}


status main(string arg) {
  int x;
  x = 0;
  if(!arg || !this_player()->query_security_level())
      arg = this_player()->query_real_name();
  if(arg != "x"){
   if(!find_player(arg)){ write("No.\n"); return 1; }
  } else {
      arg = this_player()->query_real_name();
      x = 1;
  }

printf("\n\%s\nCurrently:\n%s",PRINTBAR, YELLOW_F); 

 if(TP->query_var("feedback") || x)
printf("feedback        : %8s           feedback time     : %O\n",
answer(TP->query_var("feedback")),TP->query_var("feedback_time"));

 if(TP->query_var("sight") || x)
printf("sight           : %8s           sight time        : %O\n",
answer(TP->query_var("sight")),TP->query_var("sight_time"));

 if(TP->query_var("fangs") || x)
printf("fangs           : %8s           fangs time        : %O\n",
answer(TP->query_var("fangs")),TP->query_var("fangs_time"));

 if(TP->query_var("claws") || x)
printf("claws           : %8s           claws time        : %O\n",
answer(TP->query_var("claws")),TP->query_var("claws_time"));

 if(TP->query_var("wings") || x)
printf("wings           : %8s           wings time        : %O\n",
answer(TP->query_var("wings")),TP->query_var("wings_time"));

 if(TP->query_var("regenerate") || x)
printf("regenerate      : %8s           regenerate time   : %O\n",
answer(TP->query_var("regenerate")),TP->query_var("regenerate_time"));

 if(TP->query_var("scales") || x)
printf("scales          : %8s           scales time       : %O\n",
answer(TP->query_var("scales")),TP->query_var("scales_time"));

 if(TP->query_var("awareness") || x)
printf("awareness       : %8s           awareness time    : %O\n",
answer(TP->query_var("awareness")),TP->query_var("awareness_time"));

 if(TP->query_var("bless") || x)
printf("bless           : %8s           bless time        : %O\n",
answer(TP->query_var("bless")),TP->query_var("bless_time"));

 if(TP->query_var("fortitude") || x)
printf("fortitude       : %8s           fortitude time    : %O\n",
answer(TP->query_var("fortitude")),TP->query_var("fortitude_time"));

 if(TP->query_var("heroism") || x)
printf("heroism         : %8s           heroism time      : %O\n",
answer(TP->query_var("heroism")),TP->query_var("heroism_time"));

 if(TP->query_var("manafuse") || x)
printf("manafuse        : %8s           manafuse time     : %O\n",
answer(TP->query_var("manafuse")),TP->query_var("manafuse_time"));

 if(TP->query_var("frenzy") || x)
printf("frenzy          : %8s           frenzy time       : %O\n",
answer(TP->query_var("frenzy")),TP->query_var("frenzy_time"));

 if(TP->query_var("absorb") || x)
printf("absorb          : %8s           absorb time       : %O\n",
answer(TP->query_var("absorb")),TP->query_var("absorb_time"));

 if(TP->query_var("avenger") || x)
printf("avenger         : %8s           avenger time      : %O\n",
answer(TP->query_var("avenger")),TP->query_var("avenger_time"));

 if(TP->query_var("defender") || x)
printf("defender        : %8s           defender time     : %O\n",
answer(TP->query_var("defender")),TP->query_var("defender_time"));

 if(TP->query_var("barrier") || x)
printf("barrier         : %8s           barrier time      : %O\n",
answer(TP->query_var("barrier")),TP->query_var("barrier_time"));

 if(TP->query_var("reflect") || x)
printf("reflect         : %8s           reflect time      : %O\n",
answer(TP->query_var("reflect")),TP->query_var("reflect_time"));

 if(TP->query_var("deflect") || x)
printf("deflect         : %8s           deflect time      : %O\n",
answer(TP->query_var("deflect")),TP->query_var("deflect_time"));

 if(TP->query_var("curse") || x) 
printf("curse           : %8s           curse time        : %O\n",
answer(TP->query_var("curse")),TP->query_var("curse_time")); 

 if(TP->query_var("weaken") || x)
printf("weaken          : %8s           weaken time       : %O\n",
answer(TP->query_var("weaken")),TP->query_var("weaken_time"));

 if(TP->query_var("stun") || x)
printf("stun            : %8s           stun time         : %O\n",
answer(TP->query_var("stun")),TP->query_var("stun_time"));

 if(TP->query_var("slow") || x)
printf("slow            : %8s           slow time         : %O\n",
answer(TP->query_var("slow")),TP->query_var("slow_time"));

 if(TP->query_var("disease") || x)
printf("disease         : %8s           disease time      : %O\n",
answer(TP->query_var("disease")),TP->query_var("disease_time"));

 if(TP->query_var("poison") || x)
printf("poison          : %8s           poison time       : %O\n",
answer(TP->query_var("poison")),TP->query_var("poison_time"));

 if(TP->query_var("blind") || x)
printf("blind           : %8s           blind time        : %O\n",
answer(TP->query_var("blind")),TP->query_var("blind_time"));

 if(TP->query_var("strength") || x)
printf("strength        : %8s           strength time     : %O\n",
answer(TP->query_var("strength")),TP->query_var("strength_time"));
 
if(TP->query_var("dexterity") || x)
printf("dexterity       : %8s           dexterity time    : %O\n",
answer(TP->query_var("dexterity")),TP->query_var("dexterity_time"));

 if(TP->query_var("agility") || x)
printf("agility         : %8s           agility time      : %O\n",
answer(TP->query_var("agility")),TP->query_var("agility_time"));
 
if(TP->query_var("stamina") || x)
printf("stamina         : %8s           stamina time      : %O\n",
answer(TP->query_var("stamina")),TP->query_var("stamina_time"));

if(TP->query_var("wisdom") || x)
printf("wisdom          : %8s           wisdom time       : %O\n",
answer(TP->query_var("wisdom")),TP->query_var("wisdom_time"));

if(TP->query_var("intellect") || x)
printf("intellect       : %8s           intellect time    : %O\n",
answer(TP->query_var("intellect")),TP->query_var("intellect_time"));

if(TP->query_var("focus") || x)
printf("focus           : %8s           focus time        : %O\n",
answer(TP->query_var("focus")),TP->query_var("focus_time"));

if(TP->query_var("primal") || x)
printf("primal          : %8s           primal time       : %O\n",
answer(TP->query_var("primal")),TP->query_var("primal_time"));



printf("\
%s\n\
%s\n",
PRINTBAR, NORM);

  TP->save_me(1);
  return 1;
}


status help(){ 
          write("Type 'affects' for a list of modifiers to you.\n");
         return 1;
     }


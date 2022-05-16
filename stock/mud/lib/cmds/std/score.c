// Ash (duh)

#include <bar.h>
#include <mudlib.h>
#include <ansi.h>
string *query_cmd_aliases() { return ({"scrr", }); }

#define VALID "/secure/valid_d"

string FIX(int what){
           if(what > 999) return    ""+what;
           if(what >  99) return   " "+what;
           if(what >   9) return  "  "+what;
           return                "   "+what;
          }

#define LM NORM+MAGENTA_F+"("
#define RM NORM+MAGENTA_F+")"
#define LG NORM+GREEN_F+"("
#define RG NORM+GREEN_F+")"
#define NG NORM+GREEN_F
#define NM NORM+MAGENTA_F
#define BM B_MAGENTA_F
#define BR B_RED_F
#define R  NORM+RED_F
#define BG B_GREEN_F
#define TP find_player(arg)


status main(string arg){
  if(!arg) arg = this_player()->query_real_name();
  if(!this_player()->query_security_level())
      arg = this_player()->query_real_name();
  if(!find_player(arg)){ write("No.\n"); return 1; }

TP->clear_static_info();

write("\n"+PRINTBAR+"\n");
printf("\
%s%s",
TP->query_player_info(),TP->query_level_string());

printf("%sYour focus is %s and %s, Your deity is %s.\n\
You are trained as %s and are of the %s race.\n",
  CYAN_F,
  BOLD+TP->query_magic()+NORM+CYAN_F,
  BOLD+TP->query_faith()+NORM+CYAN_F,
  BOLD+capitalize(TP->query_alignment())+NORM+CYAN_F,
  BOLD+TP->query_class()+NORM+CYAN_F, 
  BOLD+TP->query_race()+NORM+CYAN_F,
  NORM);

printf("%s\
Health       :[ %4s%4s%s%4s%s%%  ]:     Mana         :[ %4s%4s%4s%s%s%%  ]:\n\
Carry        :[ %4s%4s%s%4s%s%%  ]:     Move         :[ %4s%4s%s%4s%s%%  ]:\n",
NM,
NM+FIX(TP->query_hp())+
LM,FIX(TP->query_max_hp())+RM,
BM,FIX(TP->query_hp_percent()),NM,

NM+FIX(TP->query_sp())+
LM,FIX(TP->query_max_sp())+RM,
BM,FIX(TP->query_sp_percent()),NM,

FIX(TP->query_cp()),
LM+FIX(TP->query_max_cp())+RM,
BM,FIX(TP->query_cp_percent()),NM,
FIX(TP->query_mp()),
LM+FIX(TP->query_max_mp())+RM,
BM,FIX(TP->query_mp_percent()),NM);

if(TP->query_race() == "avatar")
printf("%s\
Warrior      :[ %s %|11d  %s ]:     Magician     :[ %s %|11d %s  ]:\n\
Rogue        :[ %s %|11d  %s ]:     Cleric       :[ %s %|11d %s  ]:\n\%s",
GREY_F,
NORM,TP->query_class("warrior"),GREY_F,
NORM,TP->query_class("magician"),GREY_F,
NORM,TP->query_class("rogue"),GREY_F,
NORM,TP->query_class("cleric"),GREY_F,
NORM);



printf("%s\
Offensive    :[ %s+%s  ]:     Defensive    :[ %s+%s  ]:\n\
Stealth      :[ %s+%s  ]:     Casting      :[ %s+%s  ]:\n",
R,
VALID->qsr(TP->query_skill("offensive tactics"),100),
BR+FIX(TP->query_opb())+R,
VALID->qsr(TP->query_skill("defensive tactics"),100),
BR+FIX(TP->query_dpb())+R,
VALID->qsr(TP->query_skill("stealth tactics"),100),
BR+FIX(TP->query_spb())+R,
VALID->qsr(TP->query_skill("casting tactics"),100),
BR+FIX(TP->query_cpb())+R,


NORM+NM); 



printf("%s\
Focus        :[ %s/%s  ]:     Stamina      :[ %s/%s  ]:\n\
Primal       :[ %s/%s  ]:     Agility      :[ %s/%s  ]:\n\
Intellect    :[ %s/%s  ]:     Dexterity    :[ %s/%s  ]:\n\
Wisdom       :[ %s/%s  ]:     Strength     :[ %s/%s  ]:\n\
%s",

NG,
VALID->qsr(TP->query_stat("focus"),100),
BG+FIX(TP->query_stat("focus"))+NG,

VALID->qsr(TP->query_stat("stamina"),100),
BG+FIX(TP->query_stat("stamina"))+NG,

VALID->qsr(TP->query_stat("primal"),100),
BG+FIX(TP->query_stat("primal"))+NG,

VALID->qsr(TP->query_stat("agility"),100),
BG+FIX(TP->query_stat("agility"))+NG,

VALID->qsr(TP->query_stat("intellect"),100),
BG+FIX(TP->query_stat("intellect"))+NG,

VALID->qsr(TP->query_stat("dexterity"),100),
BG+FIX(TP->query_stat("dexterity"))+NG,

VALID->qsr(TP->query_stat("wisdom"),100),
BG+FIX(TP->query_stat("wisdom"))+NG,

VALID->qsr(TP->query_stat("strength"),100),
BG+FIX(TP->query_stat("strength"))+NG,

NORM+NM); 





printf("\%s%s%s%s%s", NORM, 
TP->query_hunger_string(), 
TP->query_money_string(),
TP->query_age_string(),
TP->query_status_string());


write(PRINTBAR+"\n");

return 1;
}

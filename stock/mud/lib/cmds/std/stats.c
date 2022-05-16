// Ash 

#include <bar.h>
#include <mudlib.h>
#include <ansi.h>

string *query_cmd_aliases() { return ({"statis","ts","totalscore" }); }


string FIX(int what){
           if(what > 999) return    ""+what;
           if(what >  99) return   " "+what;
           if(what >   9) return  "  "+what;
           return                "   "+what;
          }

#define TP find_player(arg)

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


int ts_age(int x){ if(x>100) return 100; return x; }

int total_score(object w){
 int x;
 x = 500; // will be used to additional varibles
 x += ts_age(w->query_days_old());
 x += w->query_alt_level();
 x += w->query_ac();
 x += w->query_wp();
 x += w->query_wc();
 x += w->query_total_skills();
 x += w->query_max_hp();
 x += w->query_max_sp();
 x += w->query_max_mp();
 x += w->query_dpb();
 x += w->query_spb();
 x += w->query_cpb();
 x += w->query_opb();
 return x/10;
}



status main(string arg){
  if(!arg) arg = this_player()->query_real_name();
  if(!this_player()->query_security_level()) 
      arg = this_player()->query_real_name();
  if(!find_player(arg)){ write("No.\n"); return 1; }

write("\n"+PRINTBAR+"\n");
printf("%s\
-----%sSTATS%s---------BASE-------BONUS-------------------MODIFIED-----------\n\
-------------------stat-------spells/items/wishes-----current/maximum---\n\
     Primal     :[ %s          (+%2d/+%2d/+%2d)            %s %3d ]:\n\
     Focus      :[ %s          (+%2d/+%2d/+%2d)            %s %3d ]:\n\
     Wisdom     :[ %s          (+%2d/+%2d/+%2d)            %s %3d ]:\n\
     Intellect  :[ %s          (+%2d/+%2d/+%2d)            %s %3d ]:\n\
     Agility    :[ %s          (+%2d/+%2d/+%2d)            %s %3d ]:\n\
     Dexterity  :[ %s          (+%2d/+%2d/+%2d)            %s %3d ]:\n\
     Stamina    :[ %s          (+%2d/+%2d/+%2d)            %s %3d ]:\n\
     Strength   :[ %s          (+%2d/+%2d/+%2d)            %s %3d ]:\n\
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n",
NG,BG,NG,
FIX(TP->query_true_stat("primal")),
TP->query_temp_stat("primal"),
TP->query_bonus_stat("primal"),
TP->query_extra_stat("primal"),
BG+TP->query_stat("primal")+NG,
TP->query_max_stat("primal"),

FIX(TP->query_true_stat("focus")),
TP->query_temp_stat("focus"),
TP->query_bonus_stat("focus"),
TP->query_extra_stat("focus"),
BG+TP->query_stat("focus")+NG,
TP->query_max_stat("focus"),

FIX(TP->query_true_stat("wisdom")),
TP->query_temp_stat("wisdom"),
TP->query_bonus_stat("wisdom"),
TP->query_extra_stat("wisdom"),
BG+TP->query_stat("wisdom")+NG,
TP->query_max_stat("wisdom"),

FIX(TP->query_true_stat("intellect")),
TP->query_temp_stat("intellect"),
TP->query_bonus_stat("intellect"),
TP->query_extra_stat("intellect"),
BG+TP->query_stat("intellect")+NG,
TP->query_max_stat("intellect"),

FIX(TP->query_true_stat("agility")),
TP->query_temp_stat("agility"),
TP->query_bonus_stat("agility"),
TP->query_extra_stat("agility"),
BG+TP->query_stat("agility")+NG,
TP->query_max_stat("agility"),

FIX(TP->query_true_stat("dexterity")),
TP->query_temp_stat("dexterity"),
TP->query_bonus_stat("dexterity"),
TP->query_extra_stat("dexterity"),
BG+TP->query_stat("dexterity")+NG,
TP->query_max_stat("dexterity"),

FIX(TP->query_true_stat("stamina")),
TP->query_temp_stat("stamina"),
TP->query_bonus_stat("stamina"),
TP->query_extra_stat("stamina"),
BG+TP->query_stat("stamina")+NG,
TP->query_max_stat("stamina"),

FIX(TP->query_true_stat("strength")),
TP->query_temp_stat("strength"),
TP->query_bonus_stat("strength"),
TP->query_extra_stat("strength"),
BG+TP->query_stat("strength")+NG,
TP->query_max_stat("strength"),
NORM+NM);

printf("%s\
 Prayers:(%d)--AltLevel:(%d)--Ego:(%d)--WP:(%d)--TotalScore:(%d)\n%s",
      BR,
      TP->shouldpray(), 
      TP->query_alt_level(), 
      TP->query_ego(),
      TP->query_wp(),
      total_score(TP),
      NORM);

write(PRINTBAR+"\n");

return 1;
  }


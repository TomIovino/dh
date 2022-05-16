//:::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: skills.h   ::  DRAGONHEART ::     ASH.11.15.96  ::
//:::::::::::::::::::::::::::::::::::::::::::::::::::::

#include <ansi.h>
 
#define wizardp(WHO)                ( ( (WHO) -> query_security() ) >= 10 )

static  string  lastskill    = "none"; 
        mapping skills       = ([ ]);
static  mapping bonus_skills = ([ ]);     
static  mapping maxskills    = ([ ]);
 
        status  set_bonus_skill(string var, mixed setting);
        status  unset_bonus_skill(string var);
        status  set_skill(string var, mixed setting);
        status  unset_skill(string var);
varargs mixed   query_skill(string var);
varargs mixed   query_max_skill(string var);
 
 
status set_bonus_skill(string var, mixed setting){
   bonus_skills[var] = setting;
   return 1;
}
status unset_bonus_skill(string var) {
    bonus_skills = m_delete(bonus_skills,var);
    return 1;
}
status start_skill(string var){
      if(!query_skill(var))
          set_skill(var,10);
       return 1;
}
status adj_skill(string var, mixed setting){
       skills[var] += setting;
       lastskill = var;
       return 1;
}
status adj_bonus_skill(string var, mixed setting){
       bonus_skills[var] += setting;
       return 1;
}
status set_skill(string var, mixed setting) {
      skills[var] = setting;
      lastskill = var;
     return 1;
}

string query_lastskill(){ 
  return lastskill;
  } 

int query_total_skills(){  return sizeof(m_indices(skills)); }
status unset_skill(string var) {
    skills = m_delete(skills,var);
    return 1;
}
 
status clear_all_skills(){ skills = ([]); return 1; }
status clear_skills(string var){
    if(!member(bonus_skills,var))
        unset_skill(var);
    else
        unset_bonus_skill(var);
    return 1;
}
 
varargs mixed query_skill(string var) {
    mixed ret;
    if(!var) return copy_mapping(skills) + copy_mapping(bonus_skills);
    ret = bonus_skills[var] + skills[var];
    if(!ret && this_object()->is_npc()) return this_object()->query_level();
    if(!ret){ skills[var] = 1; return 1; } 
    if(ret > 100){ ret = 100; skills[var] = 100; }  
    if(ret > query_max_skill(var)) ret = query_max_skill(var);
   return ret;
}
int query_skills_total(){
    int total;
    total = 0;
    return total;
}
varargs mixed query_bonus_skill(string var){
    mixed ret;
    if(!var) return copy_mapping(bonus_skills);
    ret = bonus_skills[var];
    return ret;
}
varargs mixed query_true_skill(string var){
    mixed ret;
    if(!var) return copy_mapping(skills);
    ret = skills[var];
    return ret;
  }
varargs mixed query_max_skill(string var){
    mixed ret;
    if(!var) return copy_mapping(maxskills);
    //return maxskills[var];
    return 100;
}

private protected static
void reduce_a_skill(string skill, int value) {
    if(value > 10)
        value--;
}
private protected static
void advance_a_skill(string skill, int value) {
    if(value < 90)
        value++;
}

void reduce_skills_by_one() {
    walk_mapping(skills, #'reduce_a_skill);
}

void advance_skills_by_one() {
    walk_mapping(skills, #'advance_a_skill);
}

void set_skill_one(string skill){
     set_skill(skill,1);
    }
void set_skills_one(){
     walk_mapping(skills, #'set_skill_one);
   }
 

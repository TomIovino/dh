// Checkskill & Fixskill Marco -Ash

void fixskill(string skill, int int_stat){
     if(!skill) return;
     if(!int_stat) return;
      if(this_object()->query_skill(skill) > int_stat)
           this_object()->set_skill(skill,int_stat);
}

void checkskill(string skill, string stat,int chance){
       int int_skill, int_stat;
       int_skill = (int)this_object()->query_skill(skill);
       int_stat  = (int)this_object()->query_stat(stat);
   if(!int_skill) return; 
   fixskill(skill,int_stat);
   chance = (int)random(chance);
   if(!chance && (int_stat > int_skill)) {
   if(query_skill(skill) < random(111)){
   tell_object(this_object(),
        "[ *** Your skill in "+skill+" has improved. *** ]\n");
   adj_skill(skill,1);
  }
 }
 return;
}

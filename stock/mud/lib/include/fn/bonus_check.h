// a function to tally/list bonuses on items.    
// -ash 

#define TO this_object()  
static int bonus; 

varargs int query_bonus(int x){
     bonus = 0;
     if(TO->is_living()){ return 0; }

    if(TO->query_opb()){
       if(x) write("This item gives +"+TO->query_opb()+" offense!\n"); 
        bonus += TO->query_opb();
       }    
    if(TO->query_dpb()){
       if(x) write("This item gives +"+TO->query_dpb()+" defense!\n"); 
        bonus += TO->query_dpb();
       }    
    if(TO->query_cpb()){
       if(x) write("This item gives +"+TO->query_cpb()+" casting!\n"); 
        bonus += TO->query_cpb();
       }    
    if(TO->query_spb()){
       if(x) write("This item gives +"+TO->query_spb()+" stealth!\n"); 
        bonus += TO->query_spb();
       }    
    if(TO->query_hit_func()){
      if(x) write("This weapon has a special hit.\n");
      bonus += 1;
      }
    if(TO->query_light_value()){
      if(x) write("This item glows with a magical light.\n");
      bonus += 1;
     }
    if(TO->query_enchant()){
      if(x) write("This item has been magical enhanced.\n"); 
     }
    if(TO->query_no_steal_flag()){
      if(x) write("This item is theft proof.\n");
      }
    if(TO->query_protection()) {
       if(x) write( "The "+ TO->query_name() +" will protect you from "+
          TO->query_protection() +" when it is worn.\n");
       bonus += 1;
        }
    if(TO->query_bonus_agi()){
       if(x) write("This item gives "+TO->query_bonus_agi()+" agility.\n"); 
        bonus += TO->query_bonus_agi();
       }    
    if(TO->query_bonus_str()){
       if(x) write("This item gives "+TO->query_bonus_str()+" strength.\n"); 
        bonus += TO->query_bonus_str();
       }    
    if(TO->query_bonus_sta()){
      if(x) write("This item gives "+TO->query_bonus_sta()+" stamina.\n");
        bonus += TO->query_bonus_sta(); 
       }
    if(TO->query_bonus_dex()){
       if(x) write("This item gives "+TO->query_bonus_dex()+" dexterity.\n");
        bonus += TO->query_bonus_dex();
       }
    if(TO->query_bonus_foc()){
       if(x) write("This item gives "+TO->query_bonus_foc()+" focus.\n"); 
        bonus += TO->query_bonus_wil();
       }    
    if(TO->query_bonus_int()){
       if(x) write("This item gives "+TO->query_bonus_int()+" intellect.\n");
        bonus += TO->query_bonus_int();
       }
    if(TO->query_bonus_wis()){
       if(x) write("This item gives "+TO->query_bonus_wis()+" wisdom.\n");
        bonus += TO->query_bonus_wis();
       }
    if(TO->query_bonus_pri()){
       if(x) write("This item gives "+TO->query_bonus_pri()+" primal.\n");
        bonus += TO->query_bonus_sen();
       }
    if(TO->query_ego()){
       if(x) write("This relic has an ego of "+TO->query_ego()+".\n");
       }
 
    if(TO->query_info()){
       if(x) write(query_info()); 
       }


     if(TO->is_living()) return 0;
return bonus; 

}



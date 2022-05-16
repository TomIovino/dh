string  last = "none"; 
        mapping stats = ([ ]);
static  mapping bonus_stats = ([ ]);      
        mapping extra_stats = ([ ]);     
        mapping maxstats = ([ ]);
        status  set_bonus_stat(string var, mixed setting);
        status  unset_bonus_stat(string var);
        status  set_stat(string var, mixed setting);
        status  set_max_stat(string var, mixed setting);
        status  unset_stat(string var);
        status  unset_extra_stat(string var);
varargs mixed   query_stat(string var);
varargs mixed   query_max_stat(string var);
varargs mixed   query_bonus_stat(string var);
varargs mixed   query_extra_stat(string var);
varargs mixed   query_true_stat(string var);

void check_extra_stats(){
      write("\n");
      write("Dexterity       "+query_extra_stat("dexterity")+"\t\t");
      write("Wisdom          "+query_extra_stat("wisdom")+"\n");
      write("Intellect       "+query_extra_stat("intellect")+"\t\t");
      write("Focus           "+query_extra_stat("focus")+"\n");
      write("Primal          "+query_extra_stat("primal")+"\t\t");
      write("Agility         "+query_extra_stat("agility")+"\n");
      write("Strength        "+query_extra_stat("strength")+"\t\t");
      write("Stamina         "+query_extra_stat("stamina")+"\n");
}
 
int query_maxstats_total(){
  int total;
    total =  query_max_stat("strength");
    total += query_max_stat("stamina");
    total += query_max_stat("focus");
    total += query_max_stat("primal");
    total += query_max_stat("dexterity");
    total += query_max_stat("agility");
    total += query_max_stat("intellect");
    total += query_max_stat("wisdom");
  return total;
}

int query_stats_total(){
  int total;
    total  =  stats["strength"];
    total +=  stats["dexterity"];
    total +=  stats["stamina"];
    total +=  stats["agility"];
    total +=  stats["wisdom"];
    total +=  stats["intellect"];
    total +=  stats["focus"];
    total +=  stats["primal"];
  return total;
}

int query_extrastats_total(){
  int total;
    total =  query_extra_stat("strength");
    total += query_extra_stat("stamina");
    total += query_extra_stat("focus");
    total += query_extra_stat("dexterity");
    total += query_extra_stat("agility");
    total += query_extra_stat("primal");
    total += query_extra_stat("intellect");
    total += query_extra_stat("wisdom");
  return total;
}
int query_bonusstats_total(){
  int total;
    total =  query_bonus_stat("strength");
    total += query_bonus_stat("stamina");
    total += query_bonus_stat("focus");
    total += query_bonus_stat("dexterity");
    total += query_bonus_stat("agility");
    total += query_bonus_stat("primal");
    total += query_bonus_stat("intellect");
    total += query_bonus_stat("wisdom");
  return total;
}

varargs mixed query_stat(string var) {
    mixed ret;
    ret = stats[var];                               
    ret += this_object()->query_magic_stat(var); // items, spells, wishes
    if(this_object()->is_npc()) return ret; 
    if(stats[var] > maxstats[var]) stats[var] = maxstats[var];
    if(stats[var] < 25) stats[var] = 25;
    if(ret > 100) ret = 100;
    return ret;
}
varargs mixed query_magic_stat(string var){ // "TEMP" +ITEMS +SPELLS +WISHES
    mixed ret;
    ret = 0;
    ret += this_object()->query_temp_stat(var);
    ret += this_object()->query_bonus_stat(var);
    ret += this_object()->query_extra_stat(var);
    if(ret > 30) ret = 30;
    return ret;
}

varargs mixed query_temp_stat(string var){ // FOR SPELLS/POTION AFFECTS
    mixed ret;
    ret = this_object()->query_var(var);
    if(ret > 10) ret = 10;
    return ret;
}
varargs mixed query_bonus_stat(string var){ // FOR ITEM AFFECTS
    mixed ret;
    if(!var) return copy_mapping(bonus_stats);
    ret = bonus_stats[var];
    if(ret > 10) ret = 10;
    return ret;
}
varargs mixed query_extra_stat(string var){ // FOR WISHES
    mixed ret;
    if(!var) return copy_mapping(extra_stats);
    ret = extra_stats[var];
    if(ret > 10) ret = 10; 
    return ret;
}
varargs mixed query_true_stat(string var){ // ACTUAL STAT
    mixed ret;
    ret = stats[var];
    return ret;
}

varargs mixed query_max_stat(string var){ // MAX STAT
    mixed ret;
    ret = maxstats[var];
    ret += this_object()->query_magic_stat(var); // items, spells, wishes
    if(this_object()->is_npc()) return 255;
    if( ret > 100) return 100;
    return ret;
}



mapping armours = ([ ]);
mapping weapons = ([ ]);

string strip_ansi(string str) { 
    catch(str = "/secure/ColorD/color_d"->color(str,0));
    return str;
}

void save() {
//    save_object("/ob_saves/mudlib/aw_tracker");
}

int restore() {
    return restore_object("/ob_saves/mudlib/aw_tracker");
}

void reset(int started) {
    if(started) 
        save();
    restore();
}


#define AW_ARMOUR 1
#define AW_WEAPON 2

void log_armour(object item);
void log_weapon(object item);



void log_item(int type, object item) {
    if(!item || !clonep(item))
        return;

    restore();
            
    switch(type) {
        case AW_ARMOUR:
            log_armour(item);
            break;
        case AW_WEAPON:
            log_weapon(item);
            break;
        default:
            break;
    }

    save();

    return;
}


void log_armour(object ob) {
    string type, id;
    int ac, bonus;
    
    if(!ob) return;
    
    type = (string) ob->query_type();
    id = (string) ob->query_IDfile();
    ac = (int) ob->query_ac();
    bonus = (int) ob->query_bonus();
    
    if(!mappingp(armours))
        armours = ([ ]);
    
    if(!mappingp(armours[type]))
        armours += ([ type : ([ ]) ]);
    
    if(!mappingp(armours[type][ac]))
        armours[type] += ([ ac : ([ ]) ]);

    if(!mappingp(armours[type][ac][bonus]))
        armours[type][ac] += ([ bonus : ([ ]) ]);
        
    armours[type][ac][bonus] += ([
        id: 
            strip_ansi(ob->query_short());
            ac;
            ob->query_weight();
            ob->query_value();
            type;
            bonus;
            ob->query_bonus_str();
            ob->query_bonus_dex();
            ob->query_bonus_con();
            ob->query_bonus_int();
            ob->query_bonus_wis();
            ob->query_bonus_cha();
            ob->query_bonus_awa();
            ob->query_bonus_ste();
            ob->query_bonus_wil();
            ob->query_bonus_fig(),
    ]);
    
}

void log_weapon(object ob) {
    string id;
    int wc, len;
    
    if(!ob) return;

    id = (string) ob->query_IDfile();
    wc = (int) ob->query_wc();
    len = (int) ob->query_length();

    if(!mappingp(weapons))
        weapons = ([ ]);

    if(!mappingp(weapons[wc]))
        weapons += ([ wc : ([ ]) ]);

    if(!mappingp(weapons[wc][len]))
        weapons[wc] += ([ len : ([ ]) ]);

    weapons += ([
        id: 
            strip_ansi(ob->query_short());
            wc;
            len;
            ob->query_weight();
            ob->query_value();
            ob->query_bonus();
            ob->query_bonus_str();
            ob->query_bonus_dex();
            ob->query_bonus_con();
            ob->query_bonus_int();
            ob->query_bonus_wis();
            ob->query_bonus_cha();
            ob->query_bonus_awa();
            ob->query_bonus_ste();
            ob->query_bonus_wil();
            ob->query_bonus_fig(),
    ]);
    
}

varargs
mapping query_weapons(int wc, int len) {
    restore();
    if(!wc) 
        return copy_mapping(weapons);
    if(!len) 
        return weapons[wc] ? copy_mapping(weapons[wc]) : 0;
    return weapons[wc][len] ? copy_mapping(weapons[wc][len]) : 0;
}

varargs
mapping query_armours(string type, int ac, int bonus) {
    restore();
    if(!type)
        return copy_mapping(armours);
    if(ac < 0)
        return armours[type] ? copy_mapping(armours[type]) : 0;
    if(bonus < 0)
        return armours[type][ac] ? copy_mapping(armours[type][ac]) : 0;
    if(!armours[type])
        return 0;
    if(!armours[type][ac])
        return 0;
    if(!armours[type][ac][bonus])
        return 0;
        
    return armours[type][ac][bonus] ? copy_mapping(armours[type][ac][bonus]) : 0;
}

#define MAX_EVAL 125000

int last_eval_cost = -1;

void reset_eval() { last_eval_cost = -1; }
int check_eval() {
    int used;
    if(last_eval_cost == -1) {
        last_eval_cost = get_eval_cost();
        return 1;
    }
    
    used = last_eval_cost - get_eval_cost();
    if(used > MAX_EVAL) {
        last_eval_cost = -1;
        return 0;
    }
    return 1;
}

void afile(string s) {
    write_file("/ALL_ARMOURS",s);
}


varargs void generate_armour_report(int started, string *types, int *acs, int *bonus,
                                 mapping armors, string t, int a, int b, string *ids, int i) {

    string short, file, type;
    int ac, wt, val, bn;
    mixed tmp;

    if(!started) {    
        rm("/ALL_ARMOURS");
        afile("******************************************************************************\n"
              "** DragonHeart LPmud Armour Listing ******************************************\n"
              "******************************************************************************\n");
    }
    if(!armors && !types)
        types = m_indices(query_armours()) - ({ 0 });
    if(!armors && !sizeof(types)) {
        afile(">>>>>>>End of Report<<<<<<<\n\n");
        return;
    }
    if(!armors && !acs) {
        acs = sort_array(m_indices(query_armours(t = types[0],-1,-1)) - ({ 0 }),#'<);
        afile(">>>>>> TYPE:  " + t + " <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
        types = types[1..];
    }
    if(!armors && !sizeof(acs)) {
        acs = 0;
        return call_out("generate_armour_report",1,1,types,acs,bonus,armors,t,a,b,ids,i);
    }
    if(!armors && !bonus) {
        bonus = sort_array(m_indices(query_armours(t,a = acs[0],-1)),#'<);
        acs = acs[1..];
    }
    if(!armors && !sizeof(bonus)) {
        bonus = 0;
        return call_out("generate_armour_report",1,1,types,acs,bonus,armors,t,a,b,ids,i);
    }
    if(!armors) {
        armors = query_armours(t,a,b = bonus[0]);
        bonus = bonus[1..];
    }
    if(!ids || !sizeof(ids))
        i = sizeof(ids = m_indices(armors));
    reset_eval();
    while(check_eval()) {
        if(i--) {
            if(mapping_contains(&short,&ac,&wt,&val,&type,&bn,&tmp,&tmp,&tmp,&tmp,&tmp,&tmp,&tmp,&tmp,&tmp,&tmp,armors,ids[i]))
                printf("+");
            else
                printf("-");
            afile(sprintf("%-=30s  %-=40s  %10s  %3d  (%4d/%4d) (+%d)\n", 
/*
                 armors[ids[i],0],
                 ids[i],
                 armors[ids[i],4],
                 armors[ids[i],1],
                 armors[ids[i],2],
                 armors[ids[i],3],
                 armors[ids[i],5]));
*/
        short,ids[i],type,ac,wt,val,bn));
       } else {
          ids = 0;
          i = 0;
          armors = 0;
          break;
       }
   }
   call_out("generate_armour_report",6,1,types,acs,bonus,armors,t,a,b,ids,i);
}
    

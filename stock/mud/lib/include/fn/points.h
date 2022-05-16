// keeps track of sp and hp and mp 

int hp;                // hp
int sp;                // sp
int mp;                // mp

int max_hp;            // max hp
int max_sp;            // max sp
int max_mp;            // max hp

static int extra;         // used for bonus crap

int set_hp(int i) { return hp = i;}
int set_sp(int i) { return sp = i; }
int set_mp(int i) { return mp = i; }

int query_max_hp(){ max_hp =  query_stat("stamina")*8;
                    max_hp += query_bonus(500);
                    max_hp += this_object()->query_var("heroism");
                    if(!max_hp) max_hp = hp; 
                    return max_hp; }
int query_max_sp(){ max_sp =  query_stat("focus")*8;
                    max_sp += this_object()->query_var("manafuse");
                    max_sp += query_bonus(500);
                    if(!max_sp) max_sp = sp; 
                    return max_sp; }
int query_max_mp(){ max_mp =  query_stat("dexterity")*8;
                    max_mp += query_bonus(100);
                    max_mp += this_object()->query_var("fortitude");
                    if(!max_mp) max_mp = mp; 
                    return max_mp; }

int adj_hp(int i) {
    hp += i;
    if(hp > query_max_hp()) hp = query_max_hp();
    if(hp < 0) hp = 0;
    return hp;
}
int adj_sp(int i){
    sp += i;
    if(sp > query_max_sp()) sp = query_max_sp();
    if(sp < 0) sp = 0;
    return sp;
}
int adj_mp(int i) {
    mp += i;
    if(mp > query_max_mp()) mp = query_max_mp();
    if(mp < 0) mp = 0;
    return mp;
}

int query_hp(){ if(hp > query_max_hp()) hp = query_max_hp(); return hp; }
int query_mp(){ if(mp > query_max_mp()) mp = query_max_mp(); return mp; }
int query_sp(){ if(sp > query_max_sp()) sp = query_max_sp(); return sp; }

int query_sp_percent(){ return 100*sp/query_max_sp(); }
int query_mp_percent(){ return 100*mp/query_max_mp(); }
int query_hp_percent(){ return 100*hp/query_max_hp(); }


int query_bonus(int max){ 
    int x;
    if(extra){
     if(extra < max) return extra;
      return max;
    }
    if(this_object()->is_player()){
    x = this_object()->query_days_old();
    if(x > 100) x = 100;   // check
    extra += x;
    x = this_object()->query_level(); 
    if(x > 100) x = 100;   // check  
    extra += x;
    x = this_object()->query_quest_points()*2;
    if(x > 50) x = 50;   // check
    extra += x;
    x = this_object()->query_member()/2;   
    if(x > 250) x = 250;
    extra += x;
    }
    if(this_object()->is_npc()){ extra = this_object()->query_level()*5; }
    return extra;
  }





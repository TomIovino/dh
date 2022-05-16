// this file is for all the magic stuff that the inherits use 
// i put in in a define since its used by multiple inherits
// used in weapon.c and weapon.c and maybe treasure.c sometimes =)

#include <mudlib.h>
#include <ansi.h>


int ladeda,sanctify,enchant,polished,spb,cpb,opb,dpb;
int light_value;
static string protection;
static string alignment;

static int WIS;
static int FOC;
static int PRI;
static int INT;
static int DEX;
static int STA;
static int AGI;
static int STR;
static string fs,bs;

// light, holy, physical, magik

string query_magic_prefix(){
       fs = NORM+GREY_F;
       if(light_value)    fs +=   "(";
       if(sanctify)       fs +=   "{";
       if(polished)       fs +=   "<";
       if(enchant)        fs +=   "[";
       return fs+""+NORM;
 
}                             
string query_magic_suffix(){
       bs = NORM+GREY_F;
       if(enchant)        bs +=   "]";
       if(polished)       bs +=   ">";
       if(sanctify)       bs +=   "}";
       if(light_value)    bs +=   ")";
       return ""+bs+NORM;
}                             

int fixme(){
    log_file("FIXITEM",
        file_name(this_object()) +" has an invalid stat modifier.\n");
        return 1;
   }

int adj_enchant(int s){
    if(!enchant) enchant = s;
    if(enchant > 5) enchant = 5; 
    return enchant; 
    }

int adj_polished(int s){
    if(!polished) polished = s;
    if(polished > 5) polished = 5;
    return polished;
 }

int adj_sanctify(int i){
    if(!sanctify) sanctify = i;
    if(sanctify > 5) sanctify = 5;
    return sanctify;
 }
void set_light(int i) {
    string long;
    if(!light_value)
    light_value = i*2;
    this_object()->adj_light(i*2);
  }

string set_alignment(string s){ alignment = s; }
int set_enchanted(int s){ adj_enchant(s); }
int set_sanctified(int s){ adj_sanctify(s); }
int set_polished(int s){ adj_polished(s); }
string set_protection(string pro){ if(pro) protection = pro; }

string query_protection(){ return protection; }
int query_sanctify(){ return sanctify; }
int query_polished(){ return polished; }
int query_enchant(){ return enchant; }

int query_cpb(){ return cpb; }
int query_spb(){ return spb; }
int query_opb(){ return opb; }
int query_dpb(){ return dpb; }

int query_bonus_str(){ return STR; }
int query_bonus_dex(){ return DEX; }
int query_bonus_agi(){ return AGI; }
int query_bonus_sta(){ return STA; }

int query_bonus_wis(){ return WIS; }
int query_bonus_int(){ return INT; }
int query_bonus_foc(){ return FOC; }
int query_bonus_pri(){ return PRI; }

int set_cpb(int i) { cpb = i; return cpb; }
int set_spb(int i) { spb = i; return spb; }
int set_opb(int i) { opb = i; return opb; }
int set_apb(int i) { opb = i; return opb; } // only used for compatability
int set_dpb(int i) { dpb = i; return dpb; }

int adj_cpb(int i) { cpb += i; return cpb; }
int adj_spb(int i) { spb += i; return spb; }
int adj_opb(int i) { opb += i; return opb; }
int adj_dpb(int i) { dpb += i; return dpb; }

int set_bonus_str(int i) { STR = i; return STR; }
int set_bonus_dex(int i) { DEX = i; return DEX; }
int set_bonus_sta(int i) { STA = i; return STA; }
int set_bonus_agi(int i) { AGI = i; return AGI; }
int set_bonus_int(int i) { INT = i; return INT; }
int set_bonus_wis(int i) { WIS = i; return WIS; }
int set_bonus_foc(int i) { FOC = i; return FOC; }
int set_bonus_pri(int i) { PRI = i; return PRI; }

int set_bonus_wil(int i) { fixme(); }
int set_bonus_sen(int i) { fixme(); }
int set_bonus_cha(int i) { fixme(); }
int set_bonus_fig(int i) { fixme(); }
int set_bonus_con(int i) { fixme(); }


void remove_bonuses(){
          if(spb) environment()->adj_spb(-spb);
          if(cpb) environment()->adj_cpb(-cpb);
          if(opb) environment()->adj_opb(-opb);
          if(dpb) environment()->adj_dpb(-dpb);

          if(STR) environment()->adj_bonus_stat("strength",-STR);
          if(AGI) environment()->adj_bonus_stat("agility",-AGI);
          if(STA) environment()->adj_bonus_stat("stamina",-STA);
          if(DEX) environment()->adj_bonus_stat("dexterity",-DEX);

          if(FOC) environment()->adj_bonus_stat("focus",-FOC);
          if(INT) environment()->adj_bonus_stat("intellect",-INT);
          if(WIS) environment()->adj_bonus_stat("wisdom",-WIS);
          if(PRI) environment()->adj_bonus_stat("primal",-PRI);
          if(protection) 
                  environment()->remove_spell_immunity(protection);
 return;
}

void add_bonuses(){
          if(cpb) environment()->adj_cpb(cpb);
          if(spb) environment()->adj_spb(spb);
          if(opb) environment()->adj_opb(opb);
          if(dpb) environment()->adj_dpb(dpb);
          if(STR) environment()->adj_bonus_stat("strength",STR);
          if(AGI) environment()->adj_bonus_stat("agility",AGI);
          if(STA) environment()->adj_bonus_stat("stamina",STA);
          if(DEX) environment()->adj_bonus_stat("dexterity",DEX);

          if(PRI) environment()->adj_bonus_stat("primal",PRI);
          if(FOC) environment()->adj_bonus_stat("focus",FOC);
          if(INT) environment()->adj_bonus_stat("intellect",INT);
          if(WIS) environment()->adj_bonus_stat("wisdom",WIS);

          if(protection) 
                  environment()->add_spell_immunity(protection);
return;
}


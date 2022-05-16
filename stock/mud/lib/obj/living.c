  ////////////////////////////////////////////////////////////
  // LIVING.C         // CRIMSONESTI @  // ASH.5.17.97      //
  ////////////////////////////////////////////////////////////

#pragma strict_types
#pragma save_types
#pragma combine_strings

#define RANKD "/secure/rank_d"
#define PARTY_D "/secure/party_d"

#include <mudlib.h>

inherit BASE;
inherit "/obj/env_vars";

#include "social.h"
#include "stats.h"
#include "skills.h"

#include <ansi.h>
#include <prompt.h>
#include <living.cfg>   
#include <living.spc>  
#include <compare.h>    
#include <ansi_tell_objects.h>    
#include <transport.h>
#include <rating.h>
#include <points.h>
#include <spell.h>
#include <varibles.h>
#include <ansi_filter.h>
#include <ansi_write.h>
#include <ansi_tell_object.h>
#include <levels.h>
#include <checkskill.h>
#include <language.h>
#include <show_eq.h>

/* 'rename'  some base_obj stuff */

#define THIS_PLAYER_WIZ   ((this_player()->query_security_level()) ? 1 : 0)
#define THIS_OBJECT_WIZ   ((this_object()->query_security_level()) ? 1 : 0)
#define PRIMARY_DEX       (int)primary_attack->query_stat("dexterity")
#define PRIMARY_BLOCK     (int)primary_attack->query_skill("block")
#define PRIMARY_LEVEL     (int)primary_attack->query_level()
#define THIS_PLAYER_PK    (string)this_player()->query_alignment()
#define THIS_OBJECT_PK    (string)this_object()->query_alignment()
#define THIS_PLAYER_AGI   (int)this_player()->query_stat("agility")
#define THIS_PLAYER_OPB   (int)this_player()->query_opb() 


int level;                        /* Mortal Level of Being */  
static status dead;               /* Is the being dead? */  
static status no_wimpy;           /* Can disable wimpy */  
int whimpy;                       /* Number of hp being will run at */  
int experience;                   /* Experience Value */  
int money;                        /* Amount Money Creature Has */  
int age;                          /* Number of heartbeats  1 = 2sec*/  
int gender;                       /* Numerical gender of being... */
static string custom;             /* Custom direction for wimpy exit */
static string tactic;             /* what tactic we using? */
static string position;           /* what position we using? */
static string visibility;         /* what visibility do we have? */
static string *spell_immune;      /* Spell Immunities */
static mixed *loaded_spell;       /* spell cast next heart beat */
static int magic_resist;          /* resistance to offensive spells */  
static status is_npc;             /* Is the being not a player? */
int is_invis;              /* Use invis_name, short() = 0 */
static int is_hiding;             /* Use invis_name, short() = 0 */
static int is_sneaking;           /* Use invis_name, short() = 0 */
static status is_alias;           /* Use alias_name */
static int spell_time;            /* spell casting counter */
static int armour_class;          /* Measure of protection */  
static object *armour_worn;       /* list of armour being worn */
static object *followers;         /* objects following me */
static int right_wc;              /* right weapon */  
static int left_wc;               /* left weapon_class */
static int opb;                   /* offensive power bonus */
static int dpb;                   /* defensive power bonus */
static int spb;                   /* stealth power */
static int cpb;                   /* casting power */
static object right_weapon;       /* obj weilded in right */
static string right_weapon_type;  /* right obj->type */
static string *right_attack_msg;  /* string array of atk msg's */
static object left_weapon;        /* obj weilded in left */
static string left_weapon_type;   /* left obj->type */
static string *left_attack_msg;   /* string array of atk msg's */
static object primary_attack;     /* main opponent */  
static object *secondary_attacks; /* list of other opponents */  
static string *sec_att_names;     /* aggressive living objects */
int wp;



status is_player() { return 0; }    
status is_living() { return 1; }
int query_security_level(){ return 0; }


int query_wp(){ if(wp < 1) wp = 1; if(wp > 9) wp = 9; return wp; }

string set_tactic(string s){ tactic = s; return s; }



string query_tactic(){  
  if(!tactic)  
  return "unfocused"; 
  return tactic;
}

string set_visibility(string s){ visibility = s; return s; }
string query_visibility(){  
  if(is_invis)
  return "invisible";
  if(!visibility) 
  return "visible";
  return visibility;
}

string set_position(string s){ position = s; return s; }
string query_position(){  
    if(!position) 
    return "standing";
    return position;
}

int set_invis(int i)     { if(i) set_visibility("invisible");
                           if(!i) set_visibility("visible");
                            return is_invis    = i;          }
int set_hiding(int i)    { if(i) set_visibility("hidden");
                           if(!i) set_visibility("visible");
                            return is_hiding   = i;          }
int set_sneaking(int i)  { if(i) set_position("sneaking");
                           if(!i) set_position("standing");
                           return is_sneaking = i;           }


status toggle_invis(){
  if(is_invis = !is_invis) {
    is_invis = query_security_level() ? (1000) : level;
    set_visibility("invisible");
    tell_object(this_object(),"You vanish.\n");
    say(YELLOW_F+query_name() +" leaves suddenly!\n"+NORM,this_object());
  }
  else {
    set_visibility("visible");
    tell_object(this_object(),"You are now visible.\n");
    say(YELLOW_F+query_name() +" arrives suddenly!\n"+NORM, this_object());
  }
  return 1;
}

void log(string file,mixed old, mixed new) {
    if(!this_player()) return;
    if(is_npc || !THIS_PLAYER_WIZ || !THIS_OBJECT_WIZ) return; 
    log_file("MAIN/"+ file, "To: "+ query_name(1) +
        ";By: "+(string)this_player()->query_name(1)+
        ((new) ?  ";Old: "+old+";New: "+new : ";"+old)+
        ";Time: "+ctime(time())+";\n");
}



int set_level(int i) { 
    log("SET_LEVEL",level,i);
    level = i;
    if(level < 1) level = 1;
    if(level > 100) level = 100;
    return level = i;
}


string *set_right_attack_msg(string *msg) { 
    return right_attack_msg = (msg) ? msg : ({});
}

string *set_attack_msg(string *msg) {
    return set_right_attack_msg(msg);
}


string *set_left_attack_msg(string *msg) { 
    return left_attack_msg = (msg) ? msg : ({});
}

status set_dead(status i)     { return dead     = (i) ? 1 : 0; }

status set_no_wimpy(status i) { return no_wimpy = (i) ? 1 : 0; }
int set_wimpy(int i)          { return whimpy   = i; }



int set_exp(int i) {
      log("SET_EXP", experience, experience+i);
      return experience = i;
}

int set_money(int i) {
    log("SET_MONEY", money, i);
    return money = i;
}


int set_gender(int i)    { return gender = ((i==2) ? 2 : ((i==1) ? 1 : 0)); }


object set_primary_attack(object ob)   { return primary_attack = ob;    }

object *set_secondary_attacks(object *arr) {
    return secondary_attacks = (arr) ? arr : ({});
}

mixed *set_loaded_spell(mixed *arr) {
    return loaded_spell = (arr) ? arr : ({});
}

string *set_spell_immune(string *arr) { 
    return spell_immune = (arr) ? arr : ({});
}

int set_magic_resist(int i) {
    return magic_resist = ((i > 100) ? 100 : ((i < 0) ? 0 : i));
}

status set_npc(status i) { return is_npc = (i) ? 1 : 0; }

status set_alias_status(status i) { return is_alias = (i) ? 1 : 0; }

int set_ac(int i)           { return armour_class = i; }
int set_armour_class(int i) { return armour_class = i; }

object *set_armour_worn(object *arr) {
    return armour_worn = (arr) ? arr : ({});
}

int set_wc(int i)                      { return right_wc = i; }
int set_right_wc(int i)                { return right_wc = i; }
object set_right_weapon(object ob)     { return right_weapon = ob;      }
string set_right_weapon_type(string s) { return right_weapon_type = s;  }


string query_right_weapon_type() {
           if(!query_right_weapon()) return "pound"; 
           return (string)query_right_weapon()->query_type(); }
int query_right_weapon_skill() {
           return (int)query_skill(query_right_weapon_type());
           }


int set_left_wc(int i)                 { return left_wc = i;   }  
object set_left_weapon(object ob)      { return left_weapon = ob;       }
string set_left_weapon_type(string s)  { return left_weapon_type = s;   }

string query_left_weapon_type() { 
           if(!query_left_weapon()) return "pound";
           return (string)query_left_weapon()->query_type(); }
int query_left_weapon_skill() {
           return (int)query_skill(query_right_weapon_type());
           }


/**************************************************************************/
/* query */

string query_current_room() { if(!environment()) return "NONE"; 
    return file_name(environment()); }

varargs string query_name(status real) {
    return (real)
    ? lower_case(name)
    : (is_invis) 
    ? capitalize(name)
    : (name)
    ? capitalize(name)
    : "";
}

string query_alt_name()          { return capitalize(alt_name); }
string query_real_name()         { return ((name) ? lower_case(name) : 0); }
string query_cap_name()          { return capitalize(query_alt_name()); }

int query_level(){   if(level > 250){ level = 250; } return level; }
int query_ego(){     return query_level()+query_stat("primal"); }
int query_exp(){     if(experience < 0) experience = 0; return experience; }

status query_dead()              { return dead;              }
status query_no_wimpy()          { return no_wimpy;          }
int query_wimpy()                { return whimpy;            }
string query_custom()            { return custom;            }
int query_money()                { return money;             }
int query_age()                  { return age;               }
int query_days_old()             { return age/43200;         }
string *query_all_spell_immune() { return spell_immune;      }
mixed *query_loaded_spell()      { return loaded_spell;      }
int query_magic_resist()         { return magic_resist;      }
status query_npc()               { return is_npc;            }
int query_invis()                { return is_invis;          }
int query_sneaking()             { return is_sneaking;       }
int query_hiding()               { return is_hiding;         }
status query_alias_status()      { return is_alias;          }
int query_ac()                   { return armour_class;      }
int query_armour_class()         { return armour_class;      }
object *query_all_followers()    { return followers;         }
object *query_armour_worn()      { return armour_worn;       }
int query_wc()                   { return right_wc;          }
int query_right_wc()             { return right_wc;          }                 
int query_left_wc()              { return left_wc;           }
object query_right_weapon()      { return right_weapon;      }
object query_left_weapon()       { return left_weapon;       }
string query_attacker() {
 return (primary_attack) ? (string)primary_attack->query_alt_name() : 0;
}
object query_attack()            { return primary_attack;    }  
object query_primary_attack()    { return primary_attack;    }
object *query_secondary_attacks(){ return secondary_attacks; }
object *query_alt_attackers()    { return secondary_attacks; }  

status two_weapons_wielded() {
    if(right_weapon && left_weapon) return 1;
    else return 0;
}
int need_to_next()          {
    int m;
    m = EXP[level] - experience;
    return m;
}

/****************************************************************************/
/* carry */  

#define MAX_CARRY ((int)query_stat("strength")*5)+(int)query_bonus(100)

int query_max_cp(){ return (MAX_CARRY); }
int query_cp(){ return query_max_cp()-query_weight(); }
int query_cp_percent(){ return 100 -( 100 * query_weight() / (MAX_CARRY)); }

int query_max_carry(){    return (MAX_CARRY);     }
int query_carry_percent() { return 100 * query_weight() / (MAX_CARRY); }

status add_weight(int wt) { 
    if(wt + weight > MAX_CARRY) return 0;  
    weight += wt;  
    return 1;  
}  

/****************************************************************************/
/* gender stuff */


int    query_gender() { return gender;      }
status query_neuter() { return !gender;     }  
status query_male()   { return gender == 1; }  
status query_female() { return gender == 2; }  
string query_gender_string () {  
       return (gender == 2) ? "female" : (gender == 1) ? "male": "neuter"; }  
string query_pronoun () {  
       return (gender == 2) ? "she" : (gender == 1) ? "he" : "it"; }  
string query_possessive () {  
       return (gender == 2) ? "her" : (gender == 1) ? "his" : "its"; }  
string query_objective () {  
       return (gender == 2) ? "her" : (gender == 1) ? "him" : "it"; }  


int is_wizard(){ return 0; }

int query_alt_id(string what){
          if(THIS_OBJECT_PK == what) return 1;
          if(query_race() == what) return 1;
          if(query_alignment() == what) return 1;
          if(query_gender_string() == what) return 1;
          if(what == "player" && is_player()) return 1;
          if(what == "wizard" && is_wizard()) return 1;
          if(what == "npc" && is_npc) return 1;
          return 0;
}


/**************************************************************************/
/* adj */

int adj_level(int i) {
    log("ADJ_LEVEL", level, level + i);
    return level += i; 
}
int adj_wimpy(int i)          { return whimpy  += i; }
int adj_exp(int i) {
      if(is_npc) return 0; 
      log("ADJ_EXP", experience, experience+i);
      return experience += i; 
}

int adj_money(int i) {
    log("ADJ_MONEY", money, money+i);
    return money += i;
}

int adj_magic_resist(int i) {
    return magic_resist += ((i > 100) ? 100 : ((i < 0) ? 0 : i));
}

int adj_ac(int i)                      { return armour_class += i; }
int adj_armour_class(int i)            { return armour_class += i; }
int adj_right_wc(int i)                { return right_wc += i;           }
int adj_left_wc(int i)                 { return left_wc += i;            }



/****************************************************************************/
/* adds */  

int add_exp(int i) {
    if(is_npc) return 0;
    if(i < 0) i = 0;
    log("ADD_EXP", experience, experience+i);
    experience += i;
    if(level < 100)
    if(experience >= EXP[level] && !is_npc) {
        tell_object(this_object(),B_GREEN_F BLINK "You have advanced a level!" NORM "\n");
        experience -= EXP[level];
        this_object()->set_level(level + 1);
        this_object()->clear_who_string();
    }  
    return i;

}

int add_money(int i) {
    log("ADD_MONEY",money,money+i);
     money += i;
}

/****************************************************************************/
/* show age in clock time */  

string show_age(status arg) {  
    int i;
    string str;
    str = "Age: ";  
    i = age;  
    if(i/302400) {
        str += (i/302400) + " weeks ";
        i = i - (i/302400)*302400;
    }
    if(i/43200) {  
        str += (i/43200) +" days ";  
        i = i - (i/43200)*43200;  
    }  
    if(i/1800) {  
        str += (i/1800) +" hours ";  
        i = i  - (i/1800)*1800;  
    }  
    if(i/30) {  
        str += (i/30) +" minutes ";  
        i = i - (i/30)*30;   
    }  
    str += (i*2) +" seconds.\n";
    if(!arg) write(str);  
    return str;
}  


/***********************************************************************/
/* spell_immune array stuff */

status query_spell_immunity(string str) {  
    if(!spell_immune) spell_immune = ({});
    str = str ? lower_case(str) : str;
    return (member_array(str, spell_immune) == -1) ? 0 : 1;
}  

status query_spell_immune(string str) {
    query_spell_immunity(str);
}

void add_spell_immunity(string str) {  
    str = str ? lower_case(str) : str;
    if(!query_spell_immunity(str) && str) spell_immune += ({ str, });
}  

void add_spell_immune(string str) {
    add_spell_immunity(str);
}


void remove_spell_immunity(string str) {  
    int i;

    if((i = member_array(str, spell_immune)) != -1) {
        spell_immune = spell_immune[0..i-1]
        + spell_immune[i+1..sizeof(spell_immune)-1];
    }
}     

void remove_spell_immune(string str) {
    remove_spell_immunity(str);
}

status spell_capture(object spell) {
    string spellname;
    spellname = (string)this_player()->query_spell_name();
  if(this_player() != this_object())
    if(spellname && query_spell_immunity(lower_case(spellname))) {
        tell_object(this_object(),"The "+ capitalize(spellname) 
          +" spell has no effect on you.\n");
        if(spell) destruct(spell);
        return 1;
    }
  if(this_player() != this_object())
    if(spellname && query_var("deflect") > random(222)){
        tell_object(this_object(),"The "+ capitalize(spellname) 
          +" spell is deflected!.\n");
        if(spell) destruct(spell);
       return 1;
    }
    return 0;
}


/**********************************************************************/


status query_followers(object obj) {
    return (member_array(obj, followers) == -1) ? 0 : 1;
}

void add_followers(object obj) {
    if(!query_followers(obj) && obj) followers += ({ obj, });
}

void remove_followers(object obj) {
    if(member_array(obj, followers) != -1)
        followers -= ({ 0, obj });
}

/************************************************************************/ 
/* secondary_attacks array  */
status query_secondary_attacker(object ob) {
    return (member_array(ob, secondary_attacks) == -1) ? 0 : 1;
}

status query_sec_att_name(string str) {
    if(!sec_att_names) sec_att_names = ({});
    return (member_array(str,sec_att_names) != -1) ? 1 : 0;
}


status add_secondary_attacker(object ob) {
    int i;
    string killer_name, killer_cap_name;

    if(ob == this_object()) return 0;
    if(!query_secondary_attacker(ob)) {
   if(ob && !ob->query_npc() && (killer_name = (string)ob->query_name(1))) {
            killer_cap_name = capitalize(killer_name);
            if(!sec_att_names) sec_att_names = ({});
            if(!query_npc() && ob->query_secondary_attack(this_object())
              && !ob->query_sec_att_name(killer_cap_name)) { 
                if((i = member_array(killer_name, sec_att_names)) != -1) {
                    sec_att_names = sec_att_names[0..(i-1)]
                    + sec_att_names[(i+1)..(sizeof(sec_att_names)-1)];
                }
                if(member_array(killer_cap_name, sec_att_names) == -1) {
                    sec_att_names += ({ killer_cap_name, });
                }
            }
            else  {
                if((i = member_array(killer_cap_name, sec_att_names)) != -1) {
                    sec_att_names = sec_att_names[0..(i-1)]
                    + sec_att_names[(i+1)..(sizeof(sec_att_names)-1)];
                }
                if(member_array(killer_name, sec_att_names) == -1) {
                    sec_att_names += ({ killer_name, });
                }
            }
        }
    }
    if(!query_secondary_attacker(ob) && ob) secondary_attacks += ({ ob, });
    return 1;
}  

void remove_secondary_attacker(object ob) {
    string killer_name;
    int i;

    if(ob && (killer_name = (string)ob->query_name(1))) {
        if(!sec_att_names) sec_att_names = ({});
        if((i = member_array(killer_name, sec_att_names)) != -1) {
            sec_att_names = sec_att_names[0..(i-1)]
            + sec_att_names[(i+1)..(sizeof(sec_att_names)-1)];
        }
        killer_name = capitalize(killer_name);
        if((i = member_array(killer_name, sec_att_names)) != -1) {
            sec_att_names = sec_att_names[0..(i-1)]
            + sec_att_names[(i+1)..(sizeof(sec_att_names)-1)];
        }

    }
    if(ob == primary_attack) primary_attack = 0;
    if((i = member_array(ob, secondary_attacks)) != -1) {  
        secondary_attacks = secondary_attacks[0..i-1]
        + secondary_attacks[i+1..sizeof(secondary_attacks)-1];
    }
}



/************************************************************************/ 
/* armour_worn array stuff */

status query_armour(object ob) {
    return (member_array(ob, armour_worn) == -1) ? 0 : 1;
}


static status filter_armour_type(object ob, string type) {
    if(!ob) return 0;
    if((string)ob->query_type() == type) return 1;
    return 0;
}

object query_armour_type(mixed arg) {
    object *obj, ob;
    string type;

    type = (objectp(arg)) ? (string)arg->query_type() : arg;
    obj = filter_array(armour_worn,"filter_armour_type",this_object(),type);
    ob = (sizeof(obj)) ? obj[0] : 0;
    return ob;
}

void add_armour(object ob){  
    if(!query_armour_type(ob) && ob) armour_worn += ({ ob, });
}  

/**************************************************************************/
/* check_spell */


varargs status check_spell(string id, object ob) {
    object env;
    if(!ob) ob = this_object();
    env = environment(ob);
    return (status)env->query_property(id) || (status)ob->query_var(id);
}

/**************************************************************************/
/* init stuff */

void reset(status arg) {
    if(arg) return;
    alt_name = DEFAULT_INVISNAME;
    speak_language = "common";
    spell_immune      = ({});
    loaded_spell      = ({});
    secondary_attacks = ({});
    armour_worn       = ({});
    right_attack_msg  = ({});
    left_attack_msg   = ({});
    languages_known   = ({ "common", });
    followers         = ({});
}  

/***************************************************************************/
/* condition */
string query_condition() {
    return (query_hp() < query_max_hp()/12) 
    ? RED_F+"looks ready to meet death."+NORM
    : (query_hp() < query_max_hp()/8)
    ? B_RED_F+"has been horribly injured."+NORM
    : (query_hp() < query_max_hp()/3)
    ? MAGENTA_F+"looks badly wounded."+NORM
    : (query_hp() < (query_max_hp()*2/3))
    ? YELLOW_F+"has sustained some minor wounds."+NORM
    : B_BLUE_F+"is "+ (query_hp() == query_max_hp()
      ? "unscathed and ": "") +    "looking strong."+NORM;

}

string query_short_condition() {
    return (query_hp() < query_max_hp()/12) 
    ? BLINK+RED_F+ "dying"+NORM
    : (query_hp() < query_max_hp()/8)
    ? B_RED_F+"horrible"+NORM
    : (query_hp() <query_max_hp()/3)
    ? MAGENTA_F+"badly"+NORM
    : (query_hp() < (query_max_hp()*2/3))
    ? YELLOW_F+"minor"+NORM
    : B_BLUE_F+"unscathed"+NORM;
}

/***************************************************************************/
/* long */

void long(status wiz) {  
    string al;
    string str;
    if(query_dead()) {
        write("You can see right through "+ query_objective() +"!\n");
        return;
    }
    ::long(wiz);
    al = "";
    if(al)
        sscanf(al,"(%s)",al);
    str = capitalize(query_pronoun());

    if(query_var("dragonkin"))
        write(str + " is a dragonkin.\n");
    else if(query_race() && query_race() != "") {
        write(str + 
   (( query_race()[0]=='a'|| query_race()[0]=='e'|| query_race()[0]=='i'||
      query_race()[0]=='o'|| query_race()[0]=='u')
            ? " is an " + query_race() +".\n"
            : " is a "  + query_race() +".\n"));
    }
    this_object()->show_scar(); 
    str += " ";  
    str += query_condition() + "\n";
    write(str);
}

/****************************************************************************/
/* id */

status id(string str) {  
    return (query_race() && str == query_race()) 
    || ::id(str);
}  


/***************************************************************************/
/* move_player */

static int __move_serial_number = 10000 + random(10000);
int query_move_serial() { return __move_serial_number; }

varargs 
status move_player(string dir_dest,mixed optional_dest_ob,status safe) {
    string domain1, domain2, tmp1, tmp2;
    int i, n;
    string dir;
    mixed dest;  
    object *who, *inv, ob, *inv2;  
    status is_light;
    string verb;
    object *following_players, old_room;


    verb = query_verb();

    this_object()->unload_spell();
    if(!optional_dest_ob) {  
        if(!sscanf(dir_dest, "%s#%s", dir, dest)) {  
            write("Move to bad dir/dest\n");  
            return 0;  
        }
    }  
    else {  
        dir = dir_dest;  
        dest = optional_dest_ob;  
    }

 if(primary_attack){
    if((random(query_stat("dexterity")))+
        random(query_level())
      < random(PRIMARY_BLOCK)/2+
        random(PRIMARY_DEX)/2+
        random(PRIMARY_LEVEL)/2 )
      {
        primary_attack->checkskill("block","strength",5);
        write("Your exit was blocked!.\n");
        say(capitalize(query_alt_name())+" tried to run away!\n");
        return 0;
      }
    } 
    if((old_room = environment())) {
        if(query_var("stun") && primary_attack){
            write(B_WHITE_F"You are too stunned to do that!\n"NORM);
            return 0;
        }
       is_light = (set_light(0) > 0) ? 1 : 0;  
       if(is_light) {
           who = all_inventory(environment());  
       if(this_object()->query_sneaking() ||
          this_object()->query_invis())          
           who = compare("level","level",who);
           if(dir == "X")  
                tell_objects (who, YELLOW_F+query_alt_name() 
                 + " leaves suddenly!\n"+NORM);
            else 
            if(present("horse_mount"))
                tell_objects (who, B_GREEN_F+query_alt_name()
                 +" rides quickly "+ dir+".\n"+NORM); 
            else  
            if(query_var("wings"))
                tell_objects (who, B_GREEN_F+query_alt_name()
                 +" flies "+ dir+".\n"+NORM); 
                
            else
                tell_objects (who, B_GREEN_F+query_alt_name()
                 +" leaves "+ dir+".\n"+NORM); 
        }
    }

    following_players = (object *)this_object()->query_all_followers();
    if(verb && following_players && old_room) {
        move_object(this_object(), dest);
        for(i = 0; i < sizeof(following_players); i++) {
            if(!following_players[i]) continue;
         if(this_player()->query_sneaking() || 
            environment(following_players[i]) != old_room) {
          tell_object(following_players[i], 
               (string)this_object()->query_alt_name()
                  +" no longer seems to be here.\n");
                this_object()->remove_followers(following_players[i]);
            }
            else {
                tell_object(following_players[i], "You follow "+
                  (string)this_object()->query_alt_name() +" "+ verb +"...\n");
                command(verb, following_players[i]);
                if(environment(following_players[i]) != environment()) {
                    this_object()->remove_followers(following_players[i]);
              tell_object(following_players[i], "You seem to lose your way.\n");
                }
            }
        }
    }
    else {
        move_object(this_object(), dest);
    }

    __move_serial_number++;

    is_light = (set_light(0) > 0) ? 1 : 0;  
        if(this_object()->query_sneaking()) 
               checkskill("sneak","dexterity",100);
    if(is_light) {
        who = all_inventory(environment());  
        if(this_object()->query_sneaking() ||
           this_object()->query_invis())          
              who = compare("level","level",who);
         if(dir == "X")  
            tell_objects(who, YELLOW_F+query_alt_name()+
             " arrives suddenly!\n"+NORM);
         else if(present("horse_mount"))
            tell_objects(who, B_GREEN_F+query_alt_name()+
             " rides in.\n"+NORM); 
         else if(query_var("wings"))
            tell_objects(who, B_GREEN_F+query_alt_name()+
             " flies in.\n"+NORM); 
         else
            tell_objects(who, B_GREEN_F+query_alt_name()+
             " arrives.\n"+NORM); 
    } 
    if(is_npc) return 1;
    if(!is_light  && !query_var("sight")) {  
        write("It is dark.\n");
        return 1;  
    }  
    if(query_var("blind") && !query_var("sight")) {
     write(GREY_F"You are blind!\n"NORM);
    return 1; }

    if(query_position() == "sleeping" && !query_var("awareness")){
     write(GREY_F"You are sleeping!\n"NORM);
    return 1; }

     ob = environment();  
     if(query_env_var("BRIEF")){
         write(ob->short(THIS_PLAYER_WIZ) +".\n");  
           show_inventory(ob);
         return 1; 
      }
        ob->long(THIS_PLAYER_WIZ);
        show_inventory(ob);

    return 1;
}  


/************************************************************************/  
// death function
void death() {
    object corpse, coins;
    object *inv;
    int i,wt,x,y;
    int myego;
    query_exp(); 
//  arena filter
    if(sscanf(query_current_room(), "%s/arena/%s", x,y) == 2) {
       for(i = 0; i < sizeof(secondary_attacks); i++) {
        if(secondary_attacks[i]) {
            secondary_attacks[i]->stop_fight(this_object());
            this_object()->unset_all_vars();  
        }
      }
       primary_attack = 0;
       secondary_attacks = ({});
       sec_att_names = ({});
       this_object()->stop_fight();
       corpse = this_object();
       if(previous_object()->query_alt_name()) 
       corpse = previous_object();
       if((string)corpse->query_alt_name() != "Oldage")
       catch("/secure/channeld/channeld"->
       tell_channel("info",query_alt_name(),
       ":was defeated by "+
       corpse->query_alt_name()+"."));
       this_object()->adj_hp(1000);
       this_object()->church();
       this_object()->init_command("arena");
      return 0;
    }


// end arena filter
// check for pk

    if(this_object()->is_player() && this_player()->is_player()){
           if(this_object()->query_wp())
              this_player()->add_wp(this_object()->query_wp());
              this_object()->sub_wp(1);
    }


// start immortal filter    
   if(this_object()->query_security_level()){
        tell_object(this_object(),"Your immortality heals you.\n");
        adj_hp(query_max_hp());
        return 0;
    }
// end immortal filter
   if(this_player()->query_level()+50 <
    query_level()){
     log_file("CHEAT",this_player()->query_name()+
          " killed "+query_name()+ " "+ctime()+"\n");
   }
 
    if((this_object()->is_player()) && (this_player() != this_object()))
    this_player()->toggle_last_pk();

    if(is_npc && level > 99){
       log_file("TOKEN/PRIDE",
       query_name()+"::"+ file_name(this_object())+" by "
       +previous_object()->query_real_name()+
       " on "+extract(ctime(),0,10)+"\n");
    }
  for(i = 0; i < sizeof(secondary_attacks); i++) {
        if(secondary_attacks[i]) {
            secondary_attacks[i]->stop_fight(this_object());
        }
  }

  corpse = efun::clone_object(CORPSE);  
  inv = all_inventory();  
  
  if(this_object()->is_living()){             // DO CORPSE
        for(i = 0; i < sizeof(inv); i++) {
            if(!inv[i]) continue;
            if(!inv[i]->drop(1)) {
            move_object(inv[i], corpse);
            wt += (int)inv[i]->query_weight();
          }  
       }
        corpse->set_max_weight(wt);
         if(money) {
          coins = clone_object(MONEY);
          coins->set_money(money);
          move_object(coins, corpse); 
          money = 0;
          }                               
     move_object(corpse, environment()); 
     }    // END CORPSE ?


     if(this_object() != this_player()){ 
 
     if(this_player()->query_env_var("AUTOLOOT"))
       command("*get all from corpse",this_player());
     else if(this_player()->query_env_var("AUTOGOLD"))
       command("*get coins from corpse",this_player());
     if(this_player()->query_env_var("AUTOBURY"))
       command("*bury corpse",this_player());

     }

    if(!this_object()->is_player())               // GIVE EXP
    if(this_player() != this_object()) {
        if(catch(PARTY_D->share_exp(query_exp() )))
            this_player()->add_exp(query_exp() );
    }                                             // END GIVE


    this_object()->death_2();
    say(query_alt_name() + " died.\n",this_object());
    if(this_player() != this_object()) 
    write("You killed "+query_alt_name()+".\n");
    if(!this_object()->is_player()) destruct(this_object());  
    if(!this_object()) return 0;
    primary_attack = 0;
    secondary_attacks = ({});
    sec_att_names = ({});
    set_dead(1);
    this_object()->stop_fight();
}

/* do damage without making attacker pointers */

static int do_damage(int dam) {
    int i,x,a;
    string y,z;
    int size;
    object attacker;
    int bonus;
    int mod;
    int chance;
    chance = 8; // !chance == win ::
    if((string)this_object()->query_tactic() == "stealth"){
     chance = 5;
    }
    if((string)this_object()->query_tactic() == "defensive"){
      bonus = this_object()->query_skill("defensive tactics")+1/20;
              checkskill("defensive tactics","agility",50);
     chance = 1;
    }

/* NO PK nopk 
   if(this_player()->is_player() && this_object()->is_player()){
   if(!sscanf(query_current_room(), "%s/arena/%s", y,z) != 2){
    if(this_player()->is_player() && this_object()->is_player()){
        this_object()->remove_secondary_attacker(this_player());
        this_player()->remove_secondary_attacker(this_object());
        return 0;
       }
     }
   }
*/ 
// end nopk 
    if(this_object()->is_npc()) if(query_dead()) set_dead(0);
    if(this_player()->query_dead() || this_object()->query_dead()){
        write("Nothing happens!\n");
        return 0;
    }
    if(!primary_attack) set_heart_beat(1);  /* start heart of npcs */
    if(this_object()->query_security() == 0){
        this_object()->set_sneaking(0);
        this_object()->set_hiding(0);
        this_object()->set_invis(0);
    }
    if(!query_security_level()){
        if(this_player() == this_object()){ return 0; }
    }
    if(dead) return 0;  
    if(!random(2))
    if(random(THIS_PLAYER_AGI) < random(query_stat("dexterity"))) {
        dam -= random(query_stat("dexterity")/10 + 1);
        dam -= random(query_dpb()); 
    }
    else {
        dam += random(THIS_PLAYER_AGI/10 + 1); 
        dam += random(THIS_PLAYER_OPB);
    }

   if(random(5))  // 1:5 we WILL TAKE A HIT 
   {
   if(!random(chance+5))
      if(this_object()->query_armour_type("shield")) 
       if(this_object()->query_skill("shield block")) 
         if(this_object()->query_skill("shield block")+bonus > random(110)){  
            tell_object(this_object(),
            NORM"You bravely "B_BLUE_F"shield block"NORM" "+
            this_player()->query_alt_name()+"'s attack!\n");  
            dam -= random(dam/2)+1;
     }  
     if(!random(chance+5))
       if(this_object()->query_skill("shield crush")) 
        if(this_object()->query_armour_type("shield")) 
         if(this_object()->query_skill("shield crush")+bonus > random(200)){  
            tell_object(this_object(),
            NORM"You defiantly "B_BLUE_F"shield crush"NORM" "+
            this_player()->query_alt_name()+" with a stunning blow!\n");  
            tell_object(this_player(),
            NORM"You have been "B_BLUE_F"shield crushed"NORM" by "+
            this_object()->query_alt_name()+"!\n");  
            this_player()->set_var("stun",2);
            this_player()->set_var("stun_time",2);
    }  
    if(!random(chance+3))
    if(this_object()->query_skill("dodge"))
    if(random((int)this_object()->query_skill("dodge"))+bonus > dam*7)
    {   tell_object(this_object(),
            NORM"You skillfully "MAGENTA_F"dodge"NORM" "+
            this_player()->query_alt_name() +"'s attack!\n"NORM);  
        return 0;  
    }
    if(!random(chance+3))
    if(this_object()->query_skill("parry") > random(100))
    {   tell_object(this_object(),
            NORM+"You "+B_GREEN_F+"parry"+NORM+" "
            +this_player()->query_alt_name() +"'s unskilled attack.\n");  
        return 0;  
    }
    if(!random(chance+3))
    if(this_object()->query_skill("sidestep") > random(100))
    {   tell_object(this_object(),
            NORM+"You "+GREY_F+"sidestep"+NORM+" "
            +this_player()->query_alt_name() +"'s slow attack.\n");  
        return 0;  
    }
    if(!random(chance+7))
    if(this_object()->query_var("absorb")){
        tell_object(this_object(),
            NORM+"You "+B_CYAN_F+"absorb"+NORM+" "
            +this_player()->query_alt_name() +"'s pathetic blow!\n");  
        return 0;  
    }
    if(!random(chance+6))
    if((int)this_object()->query_var("reflect") > random(50)){
        tell_object(this_object(),
            NORM+"You "+BOLD+"reflect"+NORM+" "
            +this_player()->query_alt_name() +"'s weak blow!\n");  
        return 0;  
    }
    if(!random(chance+5))
    if(this_object()->query_var("defender") > random(50)){
        tell_object(this_object(),
            NORM+"Your defender shield intercepts "
            +this_player()->query_alt_name() +"'s attack!\n"NORM);  
        tell_object(this_player(),
            query_color()+"Your attack is intercepted by "+ 
            query_alt_name()+"'s "+
            " floating defender shield.\n"NORM);
            return 0;
    }
    if(!random(chance+4))
    if(random((int)this_object()->query_var("avenger")) > random(50)){
        tell_object(this_object(),
            NORM+"Your avenger sword"+NORM+" rips "
            +this_player()->query_alt_name()+" up!\n");  
        tell_object(this_player(),
            query_color()+"You swing at "+query_alt_name()+" and are "
            +"sliced hard with a floating avenger sword.\n");
            this_player()->do_damage(25);
    }
    if(random((int)this_object()->query_var("feedback")) > random(50)){
        tell_object(this_object(),
            BLINK+GREY_F+"Your feedback shockes "
            +this_player()->query_alt_name()+".\n"+NORM);  
        tell_object(this_player(),
            query_color()+"You swing at "+query_alt_name()+" and are "
            +"shocked with feedback!\n");
            this_player()->do_damage(50);
    }
    if(!random(100) && (a = sizeof(armour_worn))){
    a = random(a);
    if(armour_worn[a])
     tell_object(this_object(),"Your "+
     (string)armour_worn[a]->short()+" is slightly damaged.\n");
    if(!is_npc && armour_worn[a]) armour_worn[a]->hit_armour(dam);
    } 
    if(this_object()->query_armour_type("shield")){
      this_object()->checkskill("shield block","strength",50);
      this_object()->checkskill("shield crush","strength",50);
    }
     this_object()->checkskill("parry","dexterity",50); 
     this_object()->checkskill("sidestep","stamina",50); 
     this_object()->checkskill("dodge","agility",50);
   } // TAKE A HIT
    mod = query_ac();
    dam -= (mod/20);
    dam -= random(mod)/20;  
    this_object()->toggle_last_combat();
    write(NORM);
    if(dam < 1) return 0;  
    if(is_player()) this_object()->linkdead();
    adj_hp(-dam);
    if(dam){
     if(!is_npc)
     tell_object(this_object(), ""+this_object()->print_prompt());
    }

    if(this_player()->query_security())
        write("["+dam+"]");
    if(query_hp() > 2) return dam;
    death();
    return dam;  
}



/***********************************************************************/
/* hit player */

int hit_player(int dam) {
    int i;
    string file;     
    string y,z;
    if(dead || this_player()->query_dead()){
       stop_fight(this_object());
       stop_fight(this_player());
        write("Nothing happens!\n");
        return 0;
    }
    // this_object is the defender
    // this_player is the attacker 
   if(this_player() == this_object()){
    remove_secondary_attacker(this_object());
   }
   if(environment(this_player())->query_no_fight()) {
            write("Fighting is Not allowed here.\n");
            this_object()->stop_fight(this_object());
            stop_fight(this_player());
            return 0;

    }
    if(add_secondary_attacker(this_player())) {
        if(!primary_attack || !present(primary_attack, environment())) {
            primary_attack = this_player();
        }
    }      
    if(!query_security_level()){
    if(this_player() == this_object()) 
    return 0;
    }
    return do_damage(dam);
}


/**************************************************************************/
/* stop fight */

varargs void stop_fight(object attacker) {
    if(!attacker) attacker = primary_attack;
    remove_secondary_attacker(attacker);
    if(attacker) attacker->remove_secondary_attacker(this_object());
}



/**************************************************************************/  
/* attack func*/  

// Attack code rewritten again on Aug 7, 98 (Ash)

void attack() {
    int dam,tdam,modifier,z;
    string file;
    string pname;
    int i, extra_attack, size;
    int rhit, lhit;
    string weapon_type,elemental,temp;  
    object alt; 
    object tmp_att;
    int bonus;
     
    if(!environment()) return;
    if(query_hp() < 1) death();

    //if(primary_attack || secondary_attacks
    //                  || sizeof(loaded_spell)) 
    
    if(query_var("stun")) return;
                       
     if(sizeof(loaded_spell))
     if(query_tactic() == "casting")
     if(query_skill("quick cast") > random(125) ){ 
          SPELL_TIME--;
          write(CYAN_F"You focus and quick cast.\n"+NORM);
     }

    if(query_carry_percent() > 50) modifier = query_carry_percent()/10; 
    if(primary_attack && primary_attack->query_dead()) primary_attack = 0;
    if(!primary_attack || !present(primary_attack,environment())) {
        for(i = 0; i < sizeof(secondary_attacks); i++) {
            if(secondary_attacks[i]) {
        if((primary_attack = present(secondary_attacks[i],environment()))) {   
                    if(!primary_attack->query_dead()) break;
                }
            }
            else {  /* remove zeros (dested attacks) */
                secondary_attacks = secondary_attacks[0..i-1]
                +secondary_attacks[i+1..sizeof(secondary_attacks)-1];
            }
        }
        if(!primary_attack || !present(primary_attack, environment())) {
            primary_attack=0;
            if(!sizeof(loaded_spell)) return;  /* no attack in room */
            else {
            if(!AREA_EFFECT && !TARGET && !PASSIVE) {
                    write("You stop casting your "+ SPELL_NAME +" spell.\n");
                    unload_spell();
                    return;
                }
            }
        }
    }

    if(primary_attack && !this_object()->query_aggressive()
      && (pname = (string)primary_attack->query_name(1))) {
        if(!sec_att_names) sec_att_names = ({});
    }

    /* cast a spell */

    if(sizeof(loaded_spell)) {
        int msg_index;
        if(++spell_time < SPELL_TIME) {
           if(pointerp(PREPARE_MSG)) {
                msg_index = (spell_time <= sizeof(PREPARE_MSG)) 
                ? spell_time-1
                : sizeof(PREPARE_MSG)-1;
                write(process_msg(PREPARE_MSG[msg_index]));
            }
            if(pointerp(PREP_MSG_ROOM)) { 
                msg_index = (spell_time <= sizeof(PREP_MSG_ROOM))
                ? spell_time-1
                : sizeof(PREP_MSG_ROOM)-1;
                say(process_msg(PREP_MSG_ROOM[msg_index]));
            }
            return;
        }
        spell_time = 0;
        cast_spell_at_target();
        return; //CAST = ENDTURN
    }

//  if(is_npc && !primary_attack) return;                  

    if((string)this_player()->query_tactic() == "offensive"){
      bonus = this_player()->query_skill("offensive tactics")+1/10;
      checkskill("offensive tactics","strength",50);
    }

  if(!primary_attack) return;

  set_invis(0);


  if(this_object()->query_skill("kick") && query_tactic() == "offensive"){
    checkskill("kick","primal",50);
    if(this_object()->query_skill("kick")+30 > random(300)){
       tdam = random(query_skill("kick"))+10/5;
       add_exp(tdam);
       write(YELLOW_F"You kick "+primary_attack->query_alt_name()+"!\n"+NORM);
       attack_msg(tdam, "crush", primary_attack, 0);
       (int)primary_attack->hit_player(tdam); 
   }
 }
  if(!primary_attack) return;

  if(this_object()->query_skill("punch") &&
   !this_object()->query_armour_type("shield") &&
    query_tactic() == "offensive"){
    checkskill("punch","primal",50);
    if(this_object()->query_skill("punch")+30 > random(300)){
       tdam = random(query_skill("punch"))+10/5;
       add_exp(tdam);
       write(YELLOW_F"You punch "+primary_attack->query_alt_name()+"!\n"+NORM);
       attack_msg(tdam, "crush", primary_attack, 0);
       (int)primary_attack->hit_player(tdam); 
   }
 }

// start weapon attacks right hand

    checkskill("enhanced damage","primal",60);

    if(right_weapon) {
        rhit = (int)right_weapon->hit(primary_attack);
        weapon_type = (right_weapon->query_type()) 
        ? (string)right_weapon->query_type()
        : "default";
    }
   if(this_object()->query_var("curse")) 
    if(((int)this_object()->query_var("curse")) && !random(10)){
        tell_object(this_object(),
            NORM"You're "GREY_F"cursed"NORM" and fumble your attack!\n");
            return 0; 
   }
   if(this_object()->query_var("bless")) 
    if(random((int)this_object()->query_var("bless")) && !random(15)){
        tell_object(this_object(),
            NORM"Your "YELLOW_F"blessing"NORM" guides your hand!\n");
            attack();
    }

// multiple attacks right hand
             
     checkskill("multi attack","stamina", 50);  

     if(!random(150) && query_right_weapon()){  
     tell_object(this_object(),"Your "+
     (string)query_right_weapon()->short()+ " is slightly damaged.\n");
     query_right_weapon()->hit_weapon(dam);
     }

     extra_attack = 0; //init multi attack int -ash
     extra_attack += query_skill("multi attack")+1/10;
     extra_attack += query_right_weapon_skill()+1/10;
     extra_attack = random(extra_attack)/5;
     if(query_var("slow") > random(200)) extra_attack -= 2;
     if(query_var("frenzy") > random(200)) extra_attack += 1;
     if(extra_attack > 4) extra_attack = 4;
     if(extra_attack < 1) extra_attack = 1;
     for(i = 0; i <= extra_attack; i++) {  

         dam = ((right_wc + rhit) + random(query_stat("agility")))/2;
         dam += random(level)/4;
         dam -= i;
         dam = (dam > 0) ? random(dam) + 1 : 0;  
     if(!primary_attack) return; 
     if(!random(10) && query_skill("critical hit")  ) {
          if(is_player()) checkskill("critical hit","dexterity", 5);
          dam += random((int)this_object()->query_stat("strength")+bonus);
          tell_object(this_object(),B_GREEN_F
                          "You scored a critical hit!\n"+NORM);
      }           
      dam = (int)primary_attack->hit_player(dam)+bonus; //right
      add_exp(dam);
      attack_msg(dam, weapon_type, primary_attack, "right");
       
      if(primary_attack && query_stat("agility")){
            if(random(query_stat("agility")) 
              < random(PRIMARY_DEX*MULTI_FREQ)) break;
        }
        else if(primary_attack) {
            if(random(query_stat("agility")) 
              < random(PRIMARY_DEX*MULTI_FREQ)) break;
        }
        else {
            break;
             }
     }

// multiple opponents right hand

    if(primary_attack && query_skill("multi opponents")){
        for(i = 0; i < sizeof(secondary_attacks); i++) {
            if((tmp_att = secondary_attacks[i])
              && present(tmp_att,environment())
              && primary_attack != tmp_att) {
              checkskill("multi opponents","stamina", 5);

             if(random(query_skill("multi opponents"))
              < random((int)tmp_att->query_skill("multi opponents")*
               MULTI_FREQ)) continue; //GSPFIX from break -> continue
           dam = ((right_wc + rhit) + 
           random(query_stat("agility")))/2;
                dam += random(level)/4;
                dam = (dam > 0) ? random(dam) + 1 : 0;  
                dam = (int)tmp_att->hit_player(dam);
                add_exp(dam);    /* get experience for just attacking */
                attack_msg(dam, weapon_type, tmp_att, "right");
            }
        }
    } 
// end right attacks
// start left weapon left attack 
  
       if(!primary_attack) return; 
       if(this_object()->query_armour_type("shield")) return; 
  if(query_skill("dual wield") +20 > random(100)){
    if(left_weapon) {
        checkskill("dual wield","dexterity", 25);
        rhit = (int)left_weapon->hit(primary_attack);
        weapon_type = (left_weapon->query_type()) 
        ? (string)left_weapon->query_type()
        : "default";
    }

// multi attacks with left weapon
              
     checkskill("multi attack","stamina", 50);  

     if(!random(150) && query_left_weapon()){
     tell_object(this_object(),"Your "+
     (string)query_left_weapon()->short()+ " is slightly damaged.\n");
     query_left_weapon()->hit_weapon(dam);
     }

     extra_attack = 0; //init multi attack int -ash
     extra_attack += query_skill("multi attack")+1/10;
     extra_attack += query_left_weapon_skill()+1/10;
     extra_attack = random(extra_attack)/5;
     if(query_var("slow") > random(200)) extra_attack -= 2;
     if(query_var("frenzy") > random(200)) extra_attack += 1;

     if(extra_attack > 2) extra_attack = 2;
     if(extra_attack < 0) extra_attack = 0;

     for(i = 0; i <= extra_attack; i++) {  
         dam = ((left_wc + rhit) + random(query_stat("agility")))/2;
         dam += random(level)/4;
         dam -= i;
         dam = (dam > 0) ? random(dam) + 1 : 0;  
       if(!primary_attack) return; 
       if(!random(10) && query_skill("critical hit")  ) {
          if(is_player()) checkskill("critical hit","dexterity", 5);
          dam += random((int)this_object()->query_stat("strength"));
          tell_object(this_object(),"You scored a critical hit!\n");
          }           
      dam = (int)primary_attack->hit_player(dam); //left
      add_exp(dam);
      attack_msg(dam, weapon_type, primary_attack, "left");
       
      if(primary_attack && query_stat("agility")){
            if(random(query_stat("agility")) 
              < random(PRIMARY_DEX*MULTI_FREQ)) break;
        }
        else if(primary_attack) {
            if(random(query_stat("agility")) 
              < random(PRIMARY_DEX*MULTI_FREQ)) break; 
        }
        else {
            break;
             }
     }

// multiple opponents with left weapon

    if(primary_attack && query_skill("multi opponents")){
        for(i = 0; i < sizeof(secondary_attacks); i++) {
           checkskill("multi opponents","stamina", 10);
            if((tmp_att = secondary_attacks[i])
              && present(tmp_att,environment())
              && primary_attack != tmp_att) {
             if(random(query_skill("multi opponents"))
              < random((int)tmp_att->query_skill("multi opponents")*
               MULTI_FREQ)) break;  // GSPFIX break -> continue
           dam = ((left_wc + rhit) + 
           random(query_stat("agility")))/2;
                dam += random(level)/4;
                dam = (dam > 0) ? random(dam) + 1 : 0;  
                dam = (int)tmp_att->hit_player(dam);
                add_exp(dam);    /* get experience for just attacking */
                attack_msg(dam, weapon_type, tmp_att, "left");
            }
        }

       //end left attacks
      }  
      //end void attack  
   } 

}


/**********************************************************************/
/* spell attacks */

/* 
* A spell undergoes 3 processes. 1. load spell 2. cast spell 3. hit by spell
*
* 1. load_spell() was design to give an easy to understand front-end
*    making the development of new spells quite easy. It easily allows
*    area effect, offensive and passive spells. And includes features
*    such as casting time, file name for cloning an object to the target,
*    and a spell component.
*
* 2. cast_spell_at_target() handles the target finding of the spell. It
*    handles both single target, and area effects.
*
* 3. spell_hit() handles the resistance checks (which can be overrided
*    by the "passive" flag in 1). It also features the capability of
*    the target to capture the spell. It also handles cloning a
*    "spell object" to the target and calls cast_spell() in the spell object.
*/

static status cast_spell_block;  /* stops inappropriate cast_spell call */


/* component extension, '#' == 'or'; '+' == 'and'
*
* eg. "component", "rope#cord+diamond#emerald",
*   will require a rope or a cord, and a diamond or an emerald for
*   spell components.
*/  

object *parse_component(string str) {
    string *pluses, *hashes, tmp;
    object *components, comp;
    int i, j, k;

    if(!str) return ({});
    components = ({});
    pluses = explode(str,"+");
    for(i = 0; i < sizeof(pluses); i++) {
        hashes = explode(pluses[i],"#");
        for(j = 0; j < sizeof(hashes); j++) {
            sscanf(hashes[j],"%s %d", hashes[j], k);
            for(k = 1, comp = 0;
              (comp = present(hashes[j] +" "+ k))
              && member_array(comp,components) != -1;
              k++);
            if(comp) {
                components += ({ comp, });
                break;
            }
        }
        if(sizeof(components)-1 != i) {  /* component not found */
            return ({});
        }
    }
    return components;
}


void unload_spell() {  
    loaded_spell = ({});
    cast_spell_block = 0;
}

/* common spell queries */

int query_spell_dmg() {  return (sizeof(loaded_spell)) ? SPELL_DAM : -1; }

string query_spell_name() { return (sizeof(loaded_spell)) ? SPELL_NAME : 0; }

mixed query_spell_argument() { return (sizeof(loaded_spell)) ? ARGUMENT : 0; }

status query_spell_area() {
    return (!sizeof(loaded_spell)) ? 0 : ((AREA_EFFECT) ? 1 : 0);
}

// fun load_spell func 

status load_spell(mixed *arr) {  /* prepare caster to cast spell */
    int i;
    string tmp1, tmp2;
    string *spelltypes;
    string used;
    int cc;
    if(!this_object()
    || !environment(this_object())
    || environment(this_object())->query_no_magic()) {
        write("You cannot cast here.\n");
        return 1;
    }
    if(sizeof(loaded_spell)) {    /* we already had a spell loaded */
        if(query_npc()) {
            cast_spell_at_target();   /* cast it anyway */
            return 1;
        }
        if(objectp(SPELL_TYPE)) {
            write("You stop casting the spell.\n");
        }
        write("You stop casting the incantation"+
          ((SPELL_NAME) ? ", "+ SPELL_NAME : "") 
          +".\nThe energy is gone.\n");
    }

         
    spell_time = 0;
    loaded_spell = allocate(SPELL_ALLOCATE);
    for(i = 0; i < sizeof(arr); i++) {
        if(!stringp(arr[i])) continue;
        switch(arr[i]) {
        case "target":                    TARGET      = arr[++i];  break;
        case "name":                      SPELL_NAME  = arr[++i];  break; 
        case "type":                      SPELL_TYPE  = arr[++i];  break;
        case "stat":                      SPELL_STAT  = arr[++i];  break;
        case "cost":                      SPELL_COST  = arr[++i];  break;
        case "damage":                    SPELL_DAM   = arr[++i];  break;
        case "immune":                    IMMUNE_TYPE = arr[++i];  break;
        case "level":                     SPELL_LEVEL = arr[++i];  break;
        case "msg target":                TARGET_MSG  = arr[++i];  break;
        case "msg room":                  ROOM_MSG    = arr[++i];  break;
        case "msg caster":                CASTER_MSG  = arr[++i];  break;
        case "spell object":              SPELL_OBJ   = arr[++i];  break;
        case "time": case "cast time":    SPELL_TIME  = arr[++i];  break;
        case "casting msg":               PREPARE_MSG = arr[++i];  break;
        case "casting msg room":        PREP_MSG_ROOM = arr[++i];  break;    
        case "component":                 COMPONENT   = arr[++i];  break;
        case "passive":                   PASSIVE     = 1;         break;
        case "aggressive":                PASSIVE     = 0;         break;
        case "argument":                  ARGUMENT    = arr[++i];  break;
        case "area":  
           AREA_EFFECT = environment() ? all_inventory(environment()) : ({}); 
           break;
        }
    }
 if(!objectp(SPELL_TYPE)) { 
        if(COMPONENT) {
            if(query_npc()) {
                COMPONENT = ({});
            }
            else {
                COMPONENT = parse_component(COMPONENT);
                if(!sizeof(COMPONENT)) {
      write(B_MAGENTA_F"You do not have the necessary components.\n"+NORM);
                    unload_spell();
                    return 0;
                }
                else {
                    i=sizeof(COMPONENT);
                    while(i--) if(living(COMPONENT[i])) {
       write(B_MAGENTA_F"You can't use a living spell component.\n"+NORM);
                            unload_spell();
                            return 0;
                        }}}}

        if(is_player() && !query_skill(SPELL_NAME)){
          write(B_MAGENTA_F+"You have not learned that spell.\n"+NORM);
           unload_spell();
           return 0;
        }
       if(this_object()->query_sp() < SPELL_COST){
          write(B_MAGENTA_F+"You lack the energy to finish your spell!\n"+NORM);
            unload_spell();
            return 0;
        }
        
       if(query_position() == "sleeping")
          { write("Zzzzz.\n"); unload_spell(); return 0; }
      
       if(this_object()->query_sp() < SPELL_COST){
          write(B_MAGENTA_F+"Your "+SPELL_NAME+" fails!!\n"+NORM);
            unload_spell();
            return 0;
        }
       this_object()->adj_sp(-SPELL_COST);
       checkskill(SPELL_NAME,SPELL_STAT, 3);
       checkskill("research","focus", 30);
     if(this_object()->query_skill("research") > random(200)) 
       checkskill(SPELL_NAME,SPELL_STAT, 3);

   if(random(query_stat(SPELL_TYPE)) + 
      random(query_skill(SPELL_NAME) +
      ((int)this_object()->query_cpb()*6))
       < random(SPELL_COST*2)){
        write(B_RED_F+"You fizzle your spell.\n"+NORM); 
        unload_spell();
        return 0;
   }
  
/*
   if(SPELL_TYPE == "primal"){
     write("You call on your primal power of "+SPELL_STAT+".\n"); }
   else if(SPELL_TYPE == "focus"){
     write("You focus your power of "+SPELL_STAT+".\n");          }
   else { write("Invalid spell.\n"); unload_spell(); return 0;     }
*/

    if(this_player() && environment(this_player()))
        if(environment(this_player())->query_no_fight() && !PASSIVE) {
            write("You cannot cast combat spells here.\n");
            this_player()->stop_fight(this_object());
            stop_fight(this_player());
            unload_spell();
            return 0;
        }
    }
    if(PREPARE_MSG && !pointerp(PREPARE_MSG)) {
        PREPARE_MSG = ({ PREPARE_MSG, });
    }
    if(PREP_MSG_ROOM && !pointerp(PREP_MSG_ROOM)) { 
        PREP_MSG_ROOM = ({ PREP_MSG_ROOM, });
    }

   if(query_skill("quantum cast"))
    if(query_tactic() == "casting")
     if(query_skill("quantum cast")+
        ( (int)this_object()->query_cpb()*3 ) >
       random(250)+1){
           this_object()->adj_sp((SPELL_COST/2));
           write(NORM+B_CYAN_F+"You focus as you cast.\n"+NORM);
       }
    if(!objectp(SPELL_TYPE)) {
        if(query_tactic() == "casting" && SPELL_NAME){
           checkskill("casting tactics","focus",10);
           checkskill("quantum cast","focus",10);
           checkskill("quiet cast","focus",10);
           checkskill("quick cast","focus",10);
        }
     if(SPELL_NAME)
        if(query_tactic() == "casting" &&
           query_skill("quiet cast") > random(125)+1 ){
           write(NORM+WHITE_F+"You begin "
           GREY_F"quiet casting"NORM" a "+
           SPELL_NAME + " spell...\n"+NORM);
        } else {
          write(NORM+WHITE_F+"You begin chanting a "+
          SPELL_NAME + " spell...\n"+NORM);
          say(NORM+WHITE_F+query_alt_name()+
          " begins chanting in an ancient language...\n"+
          NORM);
          if(query_invis()) toggle_invis();
        }
      }
    return 1; 
}

void cast_spell_at_target() {   /* find target */
    object ob, *env, first_ob;
    int i, size, spell_dam;
    int player_enter_flag;  /* player has entered after area spell started */
    string who, spell_name;
    status check_flag;
    status saved;
    if(!sizeof(loaded_spell)) return;  /* no spell loaded */
    if(cast_spell_block) return;       /* stop inappropriate recursive calls */
    cast_spell_block = 1;  

    if(query_invis()) toggle_invis();

    if(AREA_EFFECT) {
        if(!pointerp(AREA_EFFECT)) {
            AREA_EFFECT = 0;
            size = 1;
        } else {
            env = AREA_EFFECT;
            AREA_EFFECT = this_object();  /* retain old behaviour */
            size = sizeof(env);
        }
    } else {
        size = 1;
    }


    for(i = 0; i < size; i++) {
        spell_dam = SPELL_DAM;
        if(AREA_EFFECT) {
            TARGET = env[i];
            if(!TARGET) continue;
            if(environment(TARGET) != environment()) continue;
            if(!living(TARGET)) continue;
        }
 
        if(!check_flag) {
            check_flag = 1;
            if(check_spell("Nulmagic")) {
                write("Something dispels the magical energy.\n"); 
                unload_spell();
                return;
            }
            if(this_object()->query_var("silence")) {
                write("A magical silence inhibits your spellcasting.\n");
                unload_spell();
                return;
            }
        } //* GSPFIX */

        spell_name = SPELL_NAME;  

        if(TARGET) {
            who = (stringp(TARGET)) ? TARGET : (string)TARGET->query_alt_name();
            if(!(ob = present(TARGET,environment()))) {
                if(!(ob = present(TARGET, this_object()))) {
                    if(TARGET == environment() || TARGET == "room") {
                        ob = environment();
                        who = "room";
                    } else {
                        if(member_array('/',who) == -1)
                            write(who+" is not here.\n");
                        else 
      write("You must remain in the same room while you cast this spell.\n");
                        unload_spell();
                        return;
                    }
                }            
            }
            if(pointerp(COMPONENT)) {   /*check/remove component list */
              for(i = 0; i < sizeof(COMPONENT); i++) {   
               if(COMPONENT[i]->query_modified() || COMPONENT[i]->is_living()){
                 write("Your spell energy dissipates into a fine mist.\n");
                        unload_spell();
                        return;
                    }
                }
            }
            TARGET = ob;
            if(!living(ob)) {  /* cast at non-living object */ 
                if(!SPELL_OBJ) {
                    write("It has no effect on "+who+".\n");
                    break;
                }
                ob = clone_object(SPELL_OBJ); 
                move_object(ob, TARGET);  /* changed from this_object() */
                if(!ob->cast_spell(this_player(),TARGET,0,spell_dam)) {
                    if(CASTER_MSG) write(process_msg(CASTER_MSG));
                    if(ROOM_MSG)   say(process_msg(ROOM_MSG), this_object());
                } else {
                    continue;
                }
            }
        }
//      } //* GSPFIX */
        if(!PASSIVE) {
            if(!TARGET) {
                if(primary_attack) {
                    if(present(primary_attack, environment())) {
                        TARGET = primary_attack;
                    } else {
                        write(primary_attack->query_alt_name()+" is not here.\n");
                        unload_spell();
                        return;
                    }
                } else {
                    write("Attack who?\n");
                    unload_spell();
                    return;
                }
            }
        } else if(!TARGET) {  /* passive && no target */
            TARGET = this_player();
        }

        if(!TARGET) continue;  /* bypass area effect anomalies */

          spell_dam = (int)TARGET->spell_hit(spell_dam,
          CASTER_MSG,
          TARGET_MSG, 
          ROOM_MSG,
          AREA_EFFECT,
          SPELL_OBJ, 
          IMMUNE_TYPE,
          SPELL_TYPE,
          PASSIVE);

        if(sizeof(loaded_spell))
            if(spell_dam != -1 && !PASSIVE)
               attack_msg(spell_dam, "spell", TARGET, 0);

        if(!sizeof(loaded_spell)) {  /* failsafe */
            unload_spell();
            return;
        }
        if(TARGET) AREA_EFFECT = TARGET; /* thus we know who spell hit last */
    }
    if(pointerp(COMPONENT)) {   /*check/remove component list */
        for(i = 0; i < sizeof(COMPONENT); i++) {   
            if(COMPONENT[i]) COMPONENT[i]->drop(1);
            if(COMPONENT[i]) destruct(COMPONENT[i]);
        }
    }
    if(objectp(SPELL_TYPE)) SPELL_TYPE->end_spell();  /* tell wand finished */
    unload_spell();  /* unload spell */
}  




/* 
* thus in process_msg(), this_object() == target, this_player() == caster 
* note that the caster will still have their spell loaded, so 
* caster->query_loaded_spell() will yield the spell cast at you.
* Also a comparison between dmg and caster->query_spell_dmg() will
* indicate whether the target saved, or resisted the spell.
*/

  int spell_hit(int dmg,   /* spell damage */
  string caster_msg,       /* message to caster */
  string target_msg,       /* message to target */
  string room_msg,         /* message to room */
  object area_eff,         /* if area effect spell, previous target */
  string fname,            /* filename of spell object */
  string immune_type,      /* immune type */
  mixed type,              /* the spell's type */
  status passive) {        /* no resistance checks */
    object alt, target_ob;
    int save;
    int z;

    if(!passive) {
        if(query_magic_resist() > random(100)) {
            dmg = 0;
        }
        if(immune_type && query_spell_immunity(immune_type)) {
            dmg = 0;
        }

    }
  
    // saving throw ;)
    if(!random(4)) dmg = random(dmg);
     
 
   if(fname) {
        target_ob = clone_object(fname);   
        /* weakness - failure will stop heart */
        move_object(target_ob, this_object());
    }
    if(spell_capture(target_ob)) {
        if(!passive && (!area_eff || this_player()->query_npc() || 
            (area_eff && query_npc()))) {
            add_secondary_attacker(this_player());
        }
        return 0;
    }


    if(target_ob) {   /*  caster,       target,    previous target, */
        if(target_ob->cast_spell(this_player(),this_object(),area_eff,dmg)){
            if(!passive && !(query_npc() && this_player()->query_npc())) {
                add_secondary_attacker(this_player());
            }
            return -1;  /* No hit_player or dmg msg */
        }
    }

    if(caster_msg && this_object() != this_player()) {
        write(process_msg(caster_msg));
    }

    if(target_msg)
        tell_object(this_object(),process_msg(target_msg));

    if(room_msg) {
        if(target_msg) {
            say(process_msg(room_msg), ({ this_object(), this_player() }));
        } else {
            say(process_msg(room_msg), this_object());
        }
    }

//if not an area effect or its an area effect and just one of the caster or
// target is an npcs, then we add caster to our attack list


    if(!passive) { // Aggro spell
        if(!(query_npc() && this_player()->query_npc())) {
            if(add_secondary_attacker(this_player())) {
                if(!primary_attack || environment(primary_attack) != environment()) {
                    primary_attack = this_player();
                }
            }
        }
    }

    return do_damage(dmg); 
}

status cpr() { 
    set_heart_beat(1); 
    return 1; 
} 


/****************************************************************************/
/* wimpy */

void random_move() {  
    object here;  
    string *exits;

    here = environment();  
    if(!here) return;
    if(no_wimpy) {  
        tell_object(this_object(),"Something stops you from running!\n");  
        return;  
    }  
    if(custom) {  
        if(query_stat("focus") >= random(25)) {  
            if(command(custom)) {
                tell_object(this_object(),"You keep your cool and leave "+
                  custom +".\n");  
            }
            else {
                tell_object(this_object(),"You cannot run "+custom+".\n");  
            }
        }  
        else {  
            tell_object(this_object(),"You fail to flee "+ custom +"!\n");  
        }
        return;
    }
    if(!(exits = (string *)environment()->query_open_exits())) {
        if(!(exits = (string *)environment()->query_dest_dir())) {
            if(!is_npc)
       say (query_alt_name()+" tried, but failed to run away.\n", this_object());  
            tell_object(this_object(),"You try to run away, but fail!\n");  
            return;
        }
    }
    if(!sizeof(exits)) {
        tell_object(this_object(),"There is no obvious way to run.\n");
        if(!is_npc)
  say(query_alt_name()+" tried, but failed to run away.\n", this_object());
        return;
    }
    catch(command(exits[(random(sizeof(exits)/2)*2) + 1], this_object()));
}  




/***************************************************************************/
/* carry, ac, wc calculators - these should fix problems */

void recalc_carry(){  
    int i, wt;  
    object *inv;  
    weight = 0;
    if(is_npc) weight = -1000000; // npcs have no weight limits.
    inv = all_inventory();  
    for(i = 0; i < sizeof(inv); i++){  
        if(!inv[i]) continue;
        wt = (int)inv[i]->query_weight();  
        if(((wt+weight) > (MAX_CARRY)) && !inv[i]->drop(1)){
            tell_object(this_object(),"You cannot carry this much!!\n"+  
              "You drop "+inv[i]->short()+"\n");
            move_object(inv[i],environment());
        }  
        else{    
            weight += wt;  
        }  
    }  
    //    this_object()->save_me(1);
}  

int query_pk_bonus(){
  int bb;
  bb = 0;
  return bb;
}


int adj_opb(int bonus){
        opb += bonus; 
        return opb;
    }
int adj_dpb(int bonus){
        dpb += bonus; 
        return dpb;
    }
int adj_cpb(int bonus){
        cpb += bonus; 
        return cpb;
    }
int adj_spb(int bonus){
        spb += bonus; 
        return spb;
    }

// func query_cpb

int query_cpb(){
    int tot;
    tot    = (query_stat("focus")/20);
    tot   += (int)this_object()->query_wp();
    if       (query_tactic() == "casting")
    tot   += query_skill("casting tactics")/10;
    if       (query_profession() == "magician")
    tot    += 10;
    if       (query_profession() == "cleric")
    tot   += 6;
    if       (query_carry_percent() > 50)
    tot   -= 10; 
    tot   += cpb;
    if       (tot < 0) 
          tot = 0;
    if       (tot > 30) 
          tot = 30;
    return tot;
}
int query_spb(){
    int tot;
    tot    = (query_stat("dexterity")/20);
    tot   += (int)this_object()->query_wp();
    if       (query_tactic() == "stealth")
    tot   += query_skill("stealth tactics")/10;
    if       (query_profession() == "rogue")
    tot   += 5;
    if       (query_carry_percent() > 50)
    tot   -= 10; 
    tot   += spb;
    if       (tot < 0) 
          tot = 0;
    if       (tot > 30) 
          tot = 30;
    return tot;
}
int query_opb(){
    int tot;
    tot    = (query_stat("strength")/20);
    tot   += (int)this_object()->query_wp();
    if       (query_tactic() == "offensive")
    tot   += query_skill("offensive tactics")/10;
    if       (query_profession() == "warrior")
    tot    += 4;
    if       (query_profession() == "rogue")
    tot   += 5;
    if       (query_carry_percent() > 50)
    tot   -= 10; 
    tot   += opb;
    if       (tot < 0) 
          tot = 0;
    if       (tot > 30) 
          tot = 30;
    return tot;
}
int query_dpb(){
    int tot;
    tot    = (query_stat("dexterity")/20);
    tot   += (int)this_object()->query_wp();
    if       (query_tactic() == "defensive")
    tot   += query_skill("defensive tactics")/10;   
    if       (query_profession() == "warrior")
    tot    += 6;
    if       (query_profession() == "cleric")
    tot   +=  4;
    if       (query_carry_percent() > 50)
    tot   -= 10; 
    tot   += dpb;
    if       (tot < 0) 
          tot = 0;
    if       (tot > 30) 
          tot = 30;
    return tot;
  return tot;
}

// func recalc_wc

void recalc_wc() {  
      int i;
      object *inv;
      right_wc = 0;
      right_weapon = 0;
      right_attack_msg  = ({});
      left_wc = 0;
      left_weapon = 0;
      left_attack_msg   = ({});
      inv = all_inventory();
  for(i = 0; i < sizeof(inv); i++) {
        if(inv[i]->query_wielded("right")) {
            if(right_weapon) {  
                inv[i]->dewield();
                continue;
            }
            right_weapon = inv[i];
            right_wc = 10;
            right_wc += (int)right_weapon->query_wc();
            right_wc += (int)query_stat("agility")/35;
            right_wc += (int)query_stat("strength")/35;
            if(right_wc >(query_stat("agility")/2)) 
               right_wc = query_stat("agility")/2;
       set_right_attack_msg((string *)inv[i]->query_attack_msg());
        }
        else if(inv[i]->query_wielded("left")) {
            if(left_weapon) {
                inv[i]->dewield();
                continue;
            }
            left_weapon = inv[i];
            left_wc = 10;
            left_wc += (int)left_weapon->query_wc();
            left_wc += (int)query_stat("agility")/35;
            left_wc += (int)query_stat("dexterity")/35;
            if(left_wc > query_stat("dexterity")/2) 
               left_wc = query_stat("dexterity")/2;
        set_left_attack_msg((string *)inv[i]->query_attack_msg());
        }
    }
    if(!right_weapon) {
            right_wc = query_stat("agility")/5;
            if(query_var("claws")) right_wc += 10;
            if(right_wc > 30) right_wc = 30;
            left_wc = right_wc;
    }
    if(right_weapon && !left_weapon) left_wc = 0;
    if(is_npc) left_wc += level*1;
    if(is_npc) right_wc += level*2;
}  


static status In_recalc_ac = 0;
status in_recalc_ac() { return In_recalc_ac; }
status clear_in_recalc_ac() { return In_recalc_ac = 0; }
status set_recalc_ac_status(status f) { return In_recalc_ac = f; }

void recalc_ac() {  
    int i;  
    object *inv;  

    In_recalc_ac = 1;
    armour_class = 0;  
    armour_worn = ({});
    inv = all_inventory();  
    for(i = 0; i < sizeof(inv); i++) {
        if(inv[i]->armour_class() > 0 && inv[i]->query_worn()) {
            if(query_armour_type(inv[i])) {
                inv[i]->drop(1);
                continue;
            }
            armour_class += (int)inv[i]->armour_class();  
            armour_worn += ({ inv[i], });
        }
    }
    if(is_npc) {
        armour_class += level*4;
    }
    if(query_var("scales")) armour_class += 20;
    armour_class += query_stat("dexterity")/2;
    In_recalc_ac = 0;
}  









/*************************************************************************/
void attack_msg(int dmg, string type, object who, string side) {  
    string *msg;
    int dam, size;  
    string str;
    if(!type) type = "pound";
    if(type == "spell") type == "enhanced damage";
    if(is_player()) checkskill(type,"agility", 70);
    if(!who) return;  /* attacker dested */

    if(dmg < 0) dmg = 0;
    msg = (left_attack_msg && side == "left" && sizeof(left_attack_msg))
    ? left_attack_msg

    : (right_attack_msg && side == "right" && sizeof(right_attack_msg))
    ? right_attack_msg

    :   (type == "tear" || type == "shred")      ?   ({  
      WHITE_F"missed"NORM,           "with a weak slash"NORM,
      YELLOW_F"lightly sliced"NORM,"with a light graze"NORM,
      YELLOW_F"tore"NORM,        "with a weak swing"NORM,
      BROWN_F"cut"NORM,    "with a fairly deep wound"NORM,  
      BROWN_F"sliced"NORM,      "with a strike to the head"NORM,  
      BROWN_F"shredded"NORM,      "with a slice to the chest"NORM,  
      RED_F"devastated"NORM,     "with tearing into the body"NORM,  
      RED_F"mutilated"NORM,      "\b, severely shredding a limb"NORM,
      B_RED_F"SHREDDED"NORM,   "\b, severely shredding a limb"NORM})


    :   (type == "slash")      ?   ({  
      WHITE_F"missed"NORM,           "with a weak slash"NORM,
      YELLOW_F"lightly touched"NORM,"with a light graze"NORM,
      YELLOW_F"wounded"NORM,        "with a weak blow"NORM,
      BROWN_F"cut"NORM,    "with a fairly deep wound"NORM,  
      BROWN_F"sliced"NORM,      "with a strike to the head"NORM,  
      BROWN_F"slashed"NORM,      "with a slice to the chest"NORM,  
      RED_F"devastated"NORM,     "with a hard slash to the body"NORM,  
      RED_F"mutilated"NORM,      "\b, severely disabling a limb"NORM,
      B_RED_F"MUTILATED"NORM,   "\b, severely disabling a limb"NORM})

    :   (type == "cleave")    ?   ({ 
      WHITE_F"missed"NORM,         "with a swing"NORM,
      YELLOW_F"cut"NORM,       "with a slight glance"NORM,
      YELLOW_F"sliced"NORM,         "somewhat, with a slow strike"NORM,
      BROWN_F"cleaved"NORM,         "with an attack to the body"NORM,
      BROWN_F"chopped"NORM,       "\b, bringing out a hunk of flesh"NORM,
      BROWN_F"mutilated"NORM,      "with a chop to the torso"NORM,
      RED_F"chopped"NORM,         "with devastating force"NORM,
      RED_F"severed"NORM,       "nearly taking off the head"NORM,
      B_RED_F"SEVERED"NORM,        "nearly taking off the head"NORM})

    :     (type == "crush")      ?     ({ 
      WHITE_F"missed"NORM,         "with a weak blow"NORM,
      YELLOW_F"grazed"NORM,        "with next to no force"NORM,
      YELLOW_F"bruised"NORM,       "slightly, with a weak strike"NORM,
      BROWN_F"hit"NORM,            "in the body"NORM,
      BROWN_F"swatted"NORM ,   "\b, doing a fair amount of damage"NORM,
      BROWN_F"crushed"NORM,        "with a hard hit to the body"NORM,
      RED_F"smashed"NORM,          "with a devastating blow"NORM,
      RED_F"desecrated"NORM,      "into a bloody mess"NORM,
      B_RED_F"DESECRATED"NORM,    "into a bloody mess"NORM})

    :     (type == "pound")      ?     ({ 
      WHITE_F"missed"NORM,         "with a weak blow"NORM,
      YELLOW_F"grazed"NORM,        "with next to no force"NORM,
      YELLOW_F"bruised"NORM,       "slightly, with a weak strike"NORM,
      BROWN_F"hit"NORM,            "in the body"NORM,
      BROWN_F"swatted"NORM,      "\b, doing a fair amount of damage"NORM,
      BROWN_F"cracked"NORM,        "with a hard hit to the body"NORM,
      RED_F"pounded"NORM,            "with a devastating blow"NORM,
      RED_F"obliterated"NORM,      "into a bloody mess"NORM,
      B_RED_F"OBLITERATED"NORM,    "into a bloody mess"NORM})

    :     (type == "thrust" )      ?     ({ 
      WHITE_F"missed"NORM,         "with a thrust"NORM,
      YELLOW_F"poked"NORM,         "without breaking the skin"NORM,
      YELLOW_F"prodded"NORM,       "with little effect"NORM,
      BROWN_F"stabbed"NORM,        "through the leg"NORM,
      BROWN_F"thrust deep into"NORM,"drawing a great amount of blood"NORM,
      BROWN_F"gouged"NORM,     "with a vicious wound to the chest"NORM,
      RED_F"impaled"NORM,      "with a very deep thrust to the chest"NORM,
      RED_F"gutted"NORM,        "straight through the body"NORM,
      B_RED_F"GUTTED"NORM,        "straight through the body"NORM})

    :     (type == "pierce" )      ?     ({ 
      WHITE_F"missed"NORM,         "with a pierce"NORM,
      YELLOW_F"poked"NORM,         "without breaking the skin"NORM,
      YELLOW_F"prodded"NORM,       "with little effect"NORM,
      BROWN_F"stabbed"NORM,        "through the leg"NORM,
      BROWN_F"thrust deep into"NORM,"drawing a great amount of blood"NORM,
      BROWN_F"gouged"NORM,     "with a vicious wound to the chest"NORM,
      RED_F"pierced"NORM,      "with a very deep thrust to the chest"NORM,
      RED_F"impaled"NORM,        "straight through the body"NORM,
      B_RED_F"IMPALED"NORM,        "straight through the body"NORM})

    :    (type == "spell" || type == "artifact")    ?    ({ 
      WHITE_F"missed"NORM,    "who resisted the arcane energies"NORM,
      YELLOW_F"lightly touched"NORM,"with a small burst of power"NORM,
      YELLOW_F"bruised"NORM,        "with a burst of magical power"NORM,
      BROWN_F"wounded"NORM,    "with a release of magical power"NORM,
      BROWN_F"swatted"NORM,     "with arcane energies"NORM,
      BROWN_F"smashed"NORM,     "with a burst of arcane energies"NORM,
      RED_F"devastated"NORM,    "with Arcanus Energeia"NORM,
      RED_F"annihilated"NORM,      "severely with Arcanus Energeia"NORM,
      B_RED_F"ANNIHILATED"NORM,    "severely with Arcanus Energeia"NORM})  

    :  ({ 
      WHITE_F"missed"NORM,     "completely"NORM, 
      YELLOW_F"brushed"NORM,   "slightly"NORM,
      YELLOW_F"grazed"NORM,    "barely doing damage"NORM,
      BROWN_F"tackled"NORM,     "with some force"NORM,
      BROWN_F"slammed"NORM,    "with a hard charge"NORM,
      BROWN_F"pummelled"NORM,  "with a solid blow"NORM,
      RED_F"clobbered"NORM,   "with a vicious blow to the head"NORM,
      RED_F"MASSACRED"NORM,      "with great force into the ground"NORM,
      B_RED_F"MASSACRED"NORM,      "with great force into the ground"NORM});

    size = sizeof(msg);
     dam = ((dmg+MSG_DAM-1)/MSG_DAM > (size/2)-1)   
     ? ((size/2)-1)
     : ((dmg+MSG_DAM-1)/MSG_DAM);  
    dam *= 2;           
    str = (left_weapon && side == "left")
    ? " using your left "+(string)left_weapon->query_name()
    : (right_weapon && side == "right")
    ? " using your right "+(string)right_weapon->query_name()
    : "";

    if(this_player() == who) {
     write("You "+ msg[dam] +" yourself"+ ((msg[dam+1] == "") ? "" : " ") +
          msg[dam+1] + str + ".\n");
     say(query_alt_name()+" "+msg[dam]+" "+who->query_objective()+
          "self"+ ((msg[dam+1] == "") ? "" : " ") + msg[dam+1] +".\n");
    }
    else { 
        write("You "+ msg[dam] +" "+ who->query_alt_name() +
          ((msg[dam+1] == "") ? "" : " ") + msg[dam+1] + str +
          ((THIS_PLAYER_WIZ) ? "" : "")+".\n");
        say(query_alt_name()+" "+msg[dam]+" "+who->query_alt_name()+
          ((msg[dam+1] == "") ? "" : " ") + msg[dam+1] +".\n", who);
        tell_object(who,query_alt_name() +" "+ msg[dam] +" you"+
          ((msg[dam+1] == "") ? "" : " ") + msg[dam+1] +
          ((who->query_security_level()) ? "" : "")+".\n");
    }
}  

/**************************************************************************/

void heal_self(int h) {
    int i;

    if(h <= 0) return;  
    adj_hp(h);  
}
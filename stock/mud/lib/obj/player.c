

#if 1
#define LOCK_OUT 90
#define ONLY_ALLOW ({ "next","raja","brenna","leela","ash" })
#endif

#if 0
#define SAVES_DISABLED
#endif

 ////////////////////////////////////////////////////////////
 // PLAYER.C         //       DH       // ASH.12.01.96     //
 // PLAYER.C         //       DH       // ASH.12.01.97     //
 // PLAYER.C         //       DH       // ASH.12.01.98     //
 // PLAYER.C         //       DH       // ASH.12.01.99     //
 // PLAYER.C         //       DH       // ASH.12.01.01     //
 ////////////////////////////////////////////////////////////

#define LAST_UPDATE 972998888
#define MASTER_EXP_VALUE 1000000 

#pragma strict_types
#pragma save_types
#pragma combine_strings
#pragma verbose_errors

#include <mudlib.h>
#include <udp.h>
#include <bar.h>

inherit "obj/living";
inherit "obj/actions";
inherit "obj/typer";           
inherit "obj/banish";
inherit "obj/rankings";     
inherit "obj/eqstate";        
inherit "obj/channel";

#include <player.spc>
#include <player.cfg>
#include <levels.h>
#include <ansi.h>
#include <ansi_catch_tell.h>
#include <points.h>
#include <restore.h>
#include <bad_file.h>
#include <colorme.h>
#include <filters.h>
#include <rankings.h>
#include <eqstate.h>
#include <prompt.h>

#define EMOTE_D "/secure/soul/emote_d"
#define VALID   "/secure/valid_d"

/**************************************************************************/

       int    security_level;  /* access privileges */ 
       int    create;          /* create time */
       int    last_update;     /* variable for fix_levels & do_update */
       int    last_on;         /* last log on */
       int    last_save;       /* last save */
       int    last_off;        /* last log off */
       int    last_death;      /* time of last death */
       int    last_combat;     /* time of last fight round */
       int    last_pk;         /* time of last pk */
       int    wp;              /* war points */
       int    member;          /* member as apposed to guests */
       int    crash_flag;      /* If == 1, then will restore from back_inv */
       int    quest_points;    /* points achieved doing quests */
       int    savings;         /* money in bank */
       int    total_exp;       /* All the exp a player has ever earned */ 
       int    master_exp;      /* Counter of every 1 MILL exp earned.*/
       int    intoxicated;     /* drunk */
       int    stuffed;         /* food */
       int    soaked;          /* thirst */
       int    scar;            /* Number of scars */
       int    deaths;          /* Number of deaths a player has had. */
       string party;           /* which experience party we belong to */
       string email;           /* private email address of player */  
       string address;         /* public address information */
       string *quests;         /* quests completed */
       string ip_number;       /* IP number */
       string called_from_ip;  /* IP number was used last time */  
       string enter_room;      /* the room you log off from */
       string password;        /* crypted password. */  
static object other_copy;      /* other me when logging in */
static status entering;        /* Tests whether move into room was an enter */
static status Restored; 
static string hint;            /* quest code */
static string current_quest;   /* quest code */
static string realname;        /* persons real name */  
static string new_text;        /* used for old editor */
static string PSF; 
static int    tick;            /* player regen */
static int    line;            /* used for old editor */
static int    time_to_save;    /* Time to autosave. */  
static int    headache;        /* used to keep track of drunkness */
static int    max_headache;    /* guess */
static int    password_check;  /* Checks for login password */

/**************************************************************************/


string query_object_type(){ return "player";  }

string set_address(string s){ address = s; }
string query_address(){ return address; }
void clear_aliases(){aliases = ([]); }

int query_last_on(){ return last_on; }
int query_last_off(){ return last_off; }


int query_master_exp() { return master_exp; }

int adj_master_exp(int x) {
  log_file("MAIN/MASTER_EXP",
  this_player()->query_real_name()+
  " adj_master_exp old:"+master_exp+" new:"+(master_exp+x)+" "+time()+"\n");
    master_exp += x;
    if(master_exp < 0)
       master_exp = 0;
    return master_exp;
}
        
int set_master_exp(int x) {
    log_file("MAIN/MASTER_EXP",
  this_player()->query_real_name()+
  " set_master_exp old:"+master_exp+" new:"+(master_exp+x)+" "+time()+"\n");
    if(x < 0)
        x = 0;
    return master_exp = x;
}


void update_master_exp() {
    master_exp += total_exp / MASTER_EXP_VALUE;
    total_exp %= MASTER_EXP_VALUE;
}


mixed set_level(int l) {
    mixed ret;
    update_player_ranking(1);  // remove player from old level board
    ret = ::set_level(l);
    update_player_ranking();   // put player on new board
    return ret;
}

int set_total(int x){
     if(x < 0) return 0;
     write_file("/log/MAIN/TOTAL",
     this_object()->query_real_name()+" has a total of "+x+"\n");
     return total_exp = x;
}

int set_member(int x) { 
     write_file("/log/MAIN/MEMBERS", 
     this_object()->query_real_name()+" is a member of "+x+"\n");
     return member = x;
}

int query_member() { return member; }

int set_age(int x){
   write_file("/log/MAIN/AGE",
   this_object()->query_real_name()+" was "+this_object()->query_age()+ 
   " now is "+x+"\n");
   return age = x;
}

int adj_tot(int x){
  log_file("MAIN/TOTAL",
  query_real_name()+" by "+x+" on "+ctime()+"\n");
  if(total_exp + x < total_exp) // It rolled over...
     total_exp = (1 << 31) - 1; // Set to MAXINT
  else
     total_exp += x;
  experience += x;
  return x;
}

int query_alt_level(){
                 return query_level()+
                        query_extrastats_total()/2;
                 }
int query_wp(){  if(security_level){ wp = 10; return wp; }
                 if(query_alt_level() < 50){ wp = 0; return 0; }
                 if(query_alt_level() > 75){ if(wp < 2) wp = 2; }
                 if(query_alt_level() > 99){ if(wp < 3) wp = 3; }
                 if(wp < 1) wp = 1; 
                 if(wp > 9) wp = 9; 
                 return wp; }

int add_wp(int c){
         if(c+1 >= query_wp()) wp += 1;
         return query_wp();
}

int sub_wp(int c){ 
         wp -= c;
         return query_wp();
}

int set_wp(int c){
         write_file("/log/MAIN/WP",
           this_object()->query_name()+wp+"set to "+c+"\n"); 
         wp = c;
        }

status is_linkdead() { return !interactive(); }
int disable() { return write("You cant do that!\n"),1; }


void add_standard_commands(){ 
   if(!prompt_str) prompt_str = "H:$h S:$s M:$m $e";
    this_object()->init_prompt();
    this_object()->init_channels(query_security());

    set_modify_command(this_object());     
    add_action(   "emote_hook",  "", 1);    // Emote_D Hook
    add_action (  "session_lock",             "slock"    ); 
    add_action (  "change_password",          "password" );
    add_action (  "email",                    "email"   );  
    add_action (  "save_character",           "save"    );  
    add_action (  "quit",                     "quit"    );
    add_action (  "kill",                     "kill"    );  
    add_action (  "change_language",          "speak"   );  
    add_action (  "look",                     "look"    );  
    add_action (  "look",                     "l"       );  
    add_action (  "cls",                      "cls"     );
    add_action (  "examine",                  "examine" );  
    add_action (  "examine",                  "exa"     );  
    add_action (  "whisper",                  "whisper" );  
    add_action (  "wimpy",                    "wimpy"   );  
    add_action (  "command_centre",           "",1);
    ACTIONS::add_action_commands();

    }


/**********************************************************************/
/* restrict command() usage                                           */

status init_command(string str) {
     if(this_object() != this_player() && this_player()) {
     if((int)this_player()->query_security_level() 
     < (int)this_object()->query_security_level()) {
      return 0;
    }
  }
  return command(str,this_object());
}



void reset(status arg) {  
    string fname;
    int i;
    ::reset(arg);
    if(arg) return;
    if(name) return;
    level = -1;  
    name = "logon";  
    long_desc = "";  
    prompt = 1;
    hp = 200;
    sp = 200;
    mp = 200;
    right_wc = 5;
    quests = ({});
    inventory = ({});
    backup_inv = ({});   
    if(!sscanf(file_name(this_object()),"%s#%d", fname,i)) return;
    if(!(fname == PLAYER || fname == WIZARD)) {
      destruct(this_object());
      return;
    }
    if(previous_object()) {
       if(query_ip_number(previous_object())) {
         if((other_copy = (object)previous_object()->query_other_copy())) {
          name = (string)previous_object()->query_name(1);
          exec(this_object(),previous_object());
          logon95("y");
          return;
      }
    }
  }
}  

/*************************************************************************/
/* IDENTIFY the player */

status id(string str) { 
    if(str == "PLAYER") return 1;
    if(this_player() != this_object())
        if(query_invis()) {
            if(this_player()) {
                if(!this_player()->query_security_level()){
                    return 0;
                }
            }
        }
  return str == query_name(1)
      || str == query_name()
      || str == lower_case(query_name());
}


/**************************************************************************/


string query_long_name(){  // =)
   string str;
   str = capitalize(query_name());
  if(query_title() != "")
   str += " "+capitalize(query_title()); 
   return str;
}
string query_short(){
   string str;
   str =     NORM;
   str +=    do_cc("");
   if(query_wp()) 
       str += "["+query_wp()+"] ";
   else str += "[*] ";
   str +=    capitalize(query_pretitle())+" "+
             query_long_name()+" "+
             " ("+query_position()+")";
   return str+NORM;
}

int query_looker(){
    int tmp;
    if((int)this_object()->query_security_level())     tmp += 111;
    return tmp;
}
int query_looked(){
    int tmp;
    if((int)this_object()->query_security_level())     tmp += 100;
    tmp += 1;   // ALLOW INVIS
    return tmp;
}

string short(status wiz) {
    string str; 
    string why;
    int LookerS, LookieS;
    why = 0;
    this_object()->linkdead();
    set_heart_beat(1);  
    LookerS =  ((int) this_player()->query_looker());
 
    LookieS =  ((int) this_object()->query_looked());

    if(this_object()->query_hiding())         why = GREY_F" (H)";
    if(this_object()->query_invis())          why = GREY_F" (I)";

    if(this_object() != this_player()) {
    if(why){
       if(LookieS < LookerS) {
             str = NORM + query_short() + why + NORM;
            } else { // u suck
             return 0;
        }
            } else { // !why
            str = query_short();
        }
            } else { // Lookie == Looker
        str = query_short();
        if(why) {    // looker wins
            str = NORM + str + why+ NORM;
        }
    }
    return str;
}



/*************************************************************************/
/* sets */

string set_name(string n)  { return ""; /* disable */ }
string set_id(string n)    { return ""; /* disable */ }
string set_short(string n) { return ""; /* disable */ }


string set_party(string s)    {
   if(s) {
        party = s;
   } else party = 0;
   return party;
}

int set_intoxicated(int i)     { return intoxicated = i;   } 
int set_stuffed(int i)         { return stuffed = i;       }
int set_soaked(int i)          { return soaked = i;        }
int set_headache(int i)        { return headache = i;      }
int set_scar(int i)            { return scar = i;          }  
string set_email(string s)     { email = s; return email; }
string set_long(string s)      { return long_desc = (s) ? s : "";   }
string set_extra(string s)     { return extra_info = (s) ? s : ""; }


string set_start() {
    string str, x,y;
    object e;
    e = environment();
    while(e && environment(e))
        e = environment(e);
    if(!e)
        return 0;
    str = file_name(e);
    if(!str || e->query_no_summon())  return 0;
    if(sscanf(str, "d/%s/%s", x,y) == 2) {
        if(x == "wizard" && !query_security_level())
         {   enter_room = ""; return 0; }
        if(x == "good" && (string)query_name() != "good")
         {   enter_room = ""; return 0; }
        if(x == "evil" && (string)query_name() != "evil")
         {   enter_room = ""; return 0; }
    }
    return enter_room = str ? str : 0;
}
  
string set_enter_room(string str){ enter_room = str; return str; }

/***************************************************************************/
/* querys */

string query_realname()   { return realname;  }  
string query_enter_room() { return enter_room; }  
string query_party() {  return party; }

string query_alt_name(){
        if(!query_invis()) 
           return capitalize(query_real_name());
        if(query_security() < 29) 
           alt_name = query_real_name();
        return capitalize(alt_name);
}

//#ifdef NO_SHADOWS
nomask status query_prevent_shadow() { return 0; }  
//#endif

string query_email()      { return email;  }  


int test_busy() {
    if(query_position() == "sleeping"){
     write(GREY_F"You are sleeping!\n"NORM);          return 1;
    }
    if(query_var("stunned")){
     write(GREY_F"You are stunned!\n"NORM); return 1;
    }
}
int test_dark() {
    if(query_position() == "sleeping"){
     write(GREY_F"You are sleeping!\n"NORM);          return 1; }
    if(query_var("sight")) {
     write(B_WHITE_F"Your eyes glow briefly.\n");      return 0; }
    if(query_var("blind")) {
     write(GREY_F"You are blind!\n"NORM);             return 1; }
    if(set_light(0) > 0){                             return 0; }
    write(GREY_F"It is dark.\n");                     return 1;
}



string query_age_string(status arg){
//  AGE * 2 = ACTUAL_SECONDS
    int i;
    string str;
    string br,yl;
    br = NORM+BROWN_F;
    yl = YELLOW_F;

    str = br+"You have been online "+yl;
    i = age;
//    if(i/302400) {
//        str += (i/302400) + " weeks ";
//        i = i - (i/302400)*302400;
//    }
    if(i/43200) {
        str += (i/43200) +" days ";
        i = i - (i/43200)*43200;
    }
     if(i){ 
        str += (i/1800) +" hours ";
        i = i  - (i/1800)*1800;
    }
   str += br;
   if((age/43200) > 100) 
      str = br+"You have been online over "+yl+"100"+br+" days"+br;
   if(!deaths)           
      str += br+" and never died, lucky you!"+br;
   else if(deaths > 100) 
      str += br+" and died over "+yl+"100"+br+" times, got scars?"+br;
   else                  
      str += br+" and died "+yl+deaths+br+" times"+br;
 return str+"\n";
}

 
 status ansi_on()         { return query_env_var("ANSI"); }
 status query_deaths()    { return deaths; }
 // func query_level 
 int query_level()        { if(level > 100) level = 100;
                            if(level < 1) level = 1;
                            return level;
                          }

int is_player()        { return 1;}

int sell(int value){
     int factor;
     int temp;
     int TrueValue;
     TrueValue = value;
     factor = query_stat("focus");
     temp = value * factor;
     value = value * 100, value = value + temp, value = value/100;
     return value-TrueValue;
   }
int buy(int value){
     int factor;
     int temp;
     int TrueValue;
     TrueValue = value;
     factor = query_stat("focus");
     temp = value * factor;
     value = value*100, value = value - temp, value = value/100;
     return value+TrueValue;
   }
string query_desc()       {  if(!long_desc) long_desc = ""; return long_desc; }  
int query_total_exp()     {  return total_exp;   } 
int query_savings()       {  return savings;     }
int query_intoxication()  {  return intoxicated; }  
int query_stuffed()       {  return stuffed;     }  
int query_soaked()        {  return soaked;      }  
object query_other_copy() { return other_copy;   }

/*************************************************************************/
/* heart beat */

static string ld_room = 0;

string get_ld_room() { return ld_room; }

void reconnect() {
     while(find_call_out("remove_linkdead") != -1)
     remove_call_out("remove_linkdead");
     tick = age+10;
     init_command("look");
     set_heart_beat(1);
     init_prompt();
     set_modify_command(this_object());
}

void linkdead() {
    if(interactive()) return;
    if(find_call_out("remove_linkdead") == -1) 
    call_out("remove_linkdead",10);
}

void remove_linkdead() {
    if(!interactive()){
        this_object()->quit_ld(0);
    }
}

static void tick(){ 
 if(age > time_to_save) {   
     save_me(0);
     time_to_save = age + 300;   
 }  
 linkdead();
 if(!interactive()){ return 0; }

  if(!secure(SEC1)) {
      if(query_idle(this_object()) > 1200){
      write(BLUE_F+"You have been idle too long.\n"+   
                   "You have been disconnected.\n"+NORM);   
      say(BLUE_F+this_object()->query_name()+
      " is struck by a bolt of lightning and vanishes!\n"+NORM);
      quit_2(1);
      return;   
     }   
   }

   if(query_var("disease")){ return; } // BUHAHAHAHA

   if(present("health rune")){
        if(query_hp() < query_max_hp()){
   write(YELLOW_F+"You feel an inner warmth as the "+
        "rune of health mends your wounds.\n"+NORM); 
        hp += 3;
      }
    }
    if(present("mana rune")){
        if(query_sp() < query_max_sp()){
    write(NORM+MAGENTA_F+"You feel your mind clear.\n"+NORM);
        sp += 1;
      }
    }
    if(present("movement rune")){
        if(query_mp() < query_max_mp()){
    write(NORM+MAGENTA_F+"You feel more rested.\n"+NORM);
        mp += 1;
      }
    }

    checkskill("fast healing","primal",50);
    checkskill("endurance","stamina",50);
    checkskill("meditate","focus",50);

    if(query_var("regenerate")) hp += 2;
    if(query_var("manafuse")) sp += 1;

    if(query_skill("fast healing") > random(100)){
             hp += 2;
             }
    if(query_skill("endurance") > random(100)){
             mp += 2;
             }
    if(query_skill("meditate") > random(100)){
             sp += 2;
             }
    if(query_position() == "kneeling"){
              sp += 8;
              mp += 2;
              hp += 2;
    }
    if(query_position() == "sleeping"){
              hp += 5;
              sp += 5;
              mp += 5;
    }
    if(query_position() == "resting"){
              hp += 4;
              sp += 4;
              mp += 4;
    }
    if(query_position() == "standing"){
              hp += 3;
              sp += 3;
              mp += 3;
    }
    if(query_stat("primal") &&
       query_stat("stamina") &&
       query_stat("agility")){
         if(query_var("poison")){
             adj_hp(-query_var("poison")/5);
             adj_mp(-query_var("poison")/5);
             adj_sp(-query_var("poison")/5);
         }
            if(stuffed){
                 sp += 2;
                 hp += 4;
                 sp += 2;
            } 
            if(intoxicated){     
                 sp += 2;
                 hp += 4;
                 mp += 2;
            }
            if(soaked){
                 sp += 4;
                 hp += 2;
                 mp += 2;
            }
     } 
     if(headache && !random(3)){
       headache -= 1;   
         if(!headache) 
         tell_object(this_object(),"You no longer have a headache.\n");   
     }   
  if(intoxicated && !random(3)){
        intoxicated -= 1;   
          if(!intoxicated) {   
              headache = max_headache;   
              max_headache = 0;   
              tell_object(this_object(),
              "You suddenly without reason get a bad headache.\n");   
         }   
     }   
  if(stuffed && !random(3)){
         stuffed -= 1;
         if(!stuffed) tell_object(this_object(),"You start to feel hungry.\n"); 
 
     } 
  if(soaked && !random(3)){
         soaked -= 1;   
         if(!soaked) tell_object(this_object(),"You begin to feel thirsty.\n"); 
   }
    adj_hp(query_stat("stamina")/20);
    adj_mp(query_stat("agility")/20);
    adj_sp(query_stat("focus")/20);

}

static void heart_beat() {   
   if(!interactive()){ return; }
   age += 1;   
   if(tick < age){ tick = age+10; this_object()->tick(); }
   this_object()->update_timed(); 
   if(!dead) attack();
   if(primary_attack && present(primary_attack,environment()) && hp < whimpy){
       if(random(2)) call_other(this_object(),"random_move");
   }
}   


/**************************************************************************/
/* access - calls valid_read(), or valid_write() in an external object 
 * modes: "r" - valid_read(), "w" - valid_write()
 */

static status access(string dir, string file, string mode) {
    if(dir && dir != "" && dir[strlen(dir)-1] == '/') {
      dir = extract(dir,0,strlen(dir)-2);
     }
    dir = dir+"/access";
    sscanf(dir,"/%s",dir);
    if(mode[0] == 'w')
     return (status)call_other(dir, "valid_write", file, query_name(1));
     else if(mode[0] == 'r')
     return (status)call_other(dir, "valid_read", file, query_name(1));
    return 0;
}


/****************************************************************************/
/* valid_write is called by edit(), write_file(), write_bytes() */

string valid_write(string str) { 
    string domain, who, file;  

    if(!str || str[0] != '/') return 0;
    str = make_path(str); 
    if(sscanf(str, WIZARD_DIR +"%s/open/%s", who, file) == 2) { 
     return extract(str,1);
    } 
    else if(sscanf(str, WIZARD_DIR +"%s/%s", who, file) == 2) {
     if(file != "access.c") {
       if(access(WIZARD_DIR+who, str, "w")) return extract(str,1);
     }
    } 
    else if(sscanf(str, DOMAIN_DIR +"%s", file) == 1) {
     if(file != "access.c") {
       if(access(DOMAIN_DIR, str,"w")) return extract(str,1);
       }
    }
    else if(sscanf(str, "/open/%s", file) == 1) { 
     return "open/" + file;  
    } 
    else if(sscanf(str, "/doc/%s", file) == 1) { 
     return "/doc/"+file;
    } 
    else if(sscanf(str, "/usr/%s", file) == 1) { /* needed for save_me() */
     if(this_object() == this_player() && secure(SEC9)) return "usr/"+ file;
    }
    return 0;  
} 



/**************************************************************************/
/* valid_read is called by file_size(), read_file(), read_bytes()
 *                         cat(), tail()
 */


string valid_read(string str) { 
    string who, file, domain;  

    if(!str) return 0;
    str = make_path(str); 
    if((file = valid_write(str))) return file;

    if(sscanf(str, "/%s.dat", file) == 1) {
     return file +".dat";
    }
    else if(sscanf(str, WIZARD_DIR +"%s/%s", who, file) == 2) { 
     if(access(WIZARD_DIR+who,str,"r")) return extract(str,1);  
    }
    else if(sscanf(str, "/doc/%s", file) == 1) { 
     return "/doc/" + file;  
    } 
    else if(sscanf(str, "/doc/help/%s",file) == 1){
     return "/doc/help/"+file;
    } 
    else if(sscanf(str, "/log/HOUSE/%s",file) == 1){
     return "/log/HOUSE/"+file;
    } 
    else if(sscanf(str, "/d/%s", file) == 1) { 
     return "d/" + file;  
    } 
    else if(sscanf(str, "/skills/%s", file) == 1) { 
     return "skills/" + file;  
    } 
    else if(sscanf(str, "/w/%s", file) == 1) { 
     return "w/" + file;  
    } 
    else if(sscanf(str, DOMAIN_DIR +"%s", file) == 1) {
     if(access(DOMAIN_DIR,str,"r")) return extract(str,1);
    }
    else if(sscanf(str,"/%s", file) == 1) {
     switch(str) {
       case "/doc/help/help":
       return file;
     }
    }
    return 0;
} 


/* wimpy */

int query_max_wimpy()    { return query_stat("focus")*2; }
int query_wimpy_percent(){ return 100*whimpy/query_max_wimpy(); }

status wimpy(string str) {   
    int i;
    if(!str || str == "0") {  
     write("You begin to feel a little more brave.\n");  
     whimpy = 0;  
     return 1;  
    }
    if(str == "direction") {
     write("You decide to flee in any direction when running from battle.\n");
     custom = 0;
     return 1;
    }  
    if(!sscanf(str,"%d",i)) {
     if(sscanf(str,"direction %s",custom)) {
         write("You will try to run "+custom+" when fleeing from battle.\n");
         return 1;
     }
     write("Usage: wimpy <hit points>\n"+
       "       wimpy direction <direction>\n");
     return 1;
    }
    if (i < 0) {  
     write("Now don't be silly.\n");  
     return 1;  
    }  
    whimpy = i;
    if(whimpy > query_max_wimpy()) whimpy =  query_max_wimpy();
    tell_object(this_object(),"Whimpy is set to "+whimpy+" hit points.\n");
    return 1;
}

/*************** session lock *************************/

static void session_lock_2(string str) {
    if(password && password != crypt(str, password)) {  
     write("\nWrong password.\n");  
     input_to("session_lock");
     return;  
    }  
    write("\nReturning to Game.\n");  
    return;  
}   

static status session_lock() {
    if(password) {
     write("Password > ");
     input_to("session_lock_2",1);
     return 1;
    }
    input_to("session_lock");
    return 1;
}

/* change password */

static status change_password() {
    if(password) {
     write("Old Password > ");
     input_to("change_password1",1);
     return 1;
    }
    change_password1("");
    return 1;
}
 
static void change_password1(string str) {
    if(password && password != crypt(str, password)) {  
     write("\nWrong old password.\n");  
     return;  
    }  
 
    input_to("change_password2", 1, 0);  
    write("\nNew password > ");  
    return;  
}   
 
static void change_password2(string str, string new){  
    if(!str || str == "") {  
     input_to("change_password2", 1, 0);  
     write("\nNew password > ");  
     return;  
    }  
    if(!new) {  
     new = str;  
     input_to("change_password2", 1, new);  
     write("\nAgain > ");  
     return;  
    }  
    if(new != str) {  
     write("\nStart Again!!\n");
     change_password2("",0);  
     return;  
    }  
    password = crypt(new, 0); /* Generate new seed */  
    write("\nPassword change Successful.\n");  
    save_me(0);  
}  


string do_desc(string long){
 long_desc = long;
 return long;
}

int query_funds(){ return query_money()+query_savings(); }

string query_money_string (){ string str,money_str; 
    money_str = "no";
    if(money) {
      if(!(money_str = (string)call_other(MONEY, "convert", money))) {
       money_str = money +"";
      }
    }
    str = BROWN_F+"You are carrying "+ money_str +NORM+BROWN_F+" coins and ";
    money_str = "no";
    if(savings) {
      if(!(money_str = (string)call_other(MONEY, "convert", savings))) {
       money_str = savings +" ";
      }
    }
   str += BROWN_F"\nYou have " + money_str +NORM+BROWN_F+ 
          " coins in the bank.\n"+NORM;
    return str;
    }



int query_rating() { 
                             return ((master_exp * 1000) + 
                             (total_exp / 1000)) * level / 100; }

int query_earned_percent(){ return to_int(((float)experience / 
                                   (float)(EXP[level]+1)) * 100);  }

int query_exp_needed(){
     int m;
     if(level == 100) return 0;
     m = EXP[level];
     return m;
}

string query_level_string(){
    int m;
    string str;
    string N,B;
    int nextlvl;
    N = NORM+CYAN_F;
    B = B_CYAN_F;
    m = EXP[level];
    nextlvl = level +1;
    str = NORM;
    if(level == 100)
    str += N+"You are level "+B+"one-hundred"+N+" with "+B+experience+N+
           " experience extra.\n";
    else 
    str += 
    N+"You are currently level "+B+level+N+" and are about "+
    B+query_earned_percent()+N+"% to level "+B+nextlvl+N+".\n"+
    "You have earned "+B+experience+N+" of the "+
    "required "+B+m+N+" to level.\n";
    str +=
"You have "+B+query_master_exp()+(query_master_exp() ? sprintf("%06d",query_total_exp()) : ""+query_total_exp())+N+
    " total experience and your rating is "+B+query_rating()+N+".\n";
  return str+NORM;
}

string query_status_string(){
    string tmp;
    tmp = BLUE_F"You are "+BOLD+query_visibility()+
      NORM+BLUE_F" and "BOLD+query_position()+NORM+BLUE_F+
      " while using "+BOLD+query_tactic()+NORM+BLUE_F+" tactics.\n"+NORM;
    return tmp+NORM;
}     
string query_hunger_string(){ 
    string tmp;
    tmp = ""BROWN_F;   
    tmp += (intoxicated && stuffed && soaked)
    ? "You are intoxicated, have a full stomach, "+
    "and are certainly not thirsty.\n"
    : (intoxicated && stuffed)
    ? "You are intoxicated, and have a full stomach.\n"
    : (intoxicated && soaked)
    ? "You are intoxicated, and are not thirsty.\n"
    : (intoxicated)
    ? "You are intoxicated.\n"
    : (stuffed && soaked)
    ? "You have a full stomach, and are certainly not thirsty.\n"
    : (stuffed)
    ? "You have a full stomach.\n"
    : (soaked)
    ? "You are certainly not thirsty.\n"
    : "You have not eaten, or drank anything lately.\n";
return tmp+NORM;
}


/********************************************************************/
/* email */

status email(){
   write("Email Address: "+email+"\n");
   write("read 'help rules' and 'help email' for information.\n");
    return 1;
}

/*************************************************************************/
/* inventory */

status inventory() { 
    if(test_dark()) return 1;
    show_inventory();
    return 1; 
} 

/*************************************************************************/
/* Equipment */

status equipment() { 
    if(test_dark()) return 1;
    show_equipment();
    return 1; 
} 

/***************************************************************************/
/* cls */

status cls(status arg) {
    int i;
    
    for (i = 35; i--;) write ("\n");
    return 1;
}

/***************************************************************************/
/* do exa */

status examine(string str) { 
    if(query_position() == "sleeping"){
     write(GREY_F"You are sleeping!\n"NORM);
     return 1; }
     if(!str) {
      write("Examine what?\n");
      return 1;
    }
    look("at "+str,0); 
    return 1;
} 

/**************************************************************************/
/* do look */

status look(string str, status extra) {
  object ob;
  string ho,sp;
  object special;
  string item;
  string extra_str;
  status wiz;
  int wt;
  wiz = secure(SEC1);
  if(test_dark()){ return 1; }
  if(!str) {
    ob = environment(this_player());
  }
  else {
    sscanf(str, "at %s", str); 
    sscanf(str, "in %s", str);
    if(environment()->look_at(str)) return 1;  

    sscanf(str, "%s on %s", item, str);
    sscanf(str, "%s in %s", item, str);
    str = lower_case(str);
    if(!(ob = present(str, this_player()))) {
      if(!(ob=present(str,environment()))) {
        write("There is no " + str + " here.\n");
        return 1;
      }
    }
    if(item) {
      if(!ob->look_at(item)) write("There is no "+ item +".\n");
      return 1;
    }
  }
  if(extra)
    ob->extra_info(wiz);
  else
    ob->long(wiz);
  if (living(ob)) {
    special = first_inventory(ob);
    while(special) {
//       extra_str = (string)special->query_extra_long();
//     if (extra_str) write(process_msg(extra_str));
    special->extra_long();

      special = next_inventory(special);
    }
  }
  if(ob == environment() || living(ob) || ob->can_put_and_get()) {
    if(!living(ob) && !extra) {
      wt = (int)ob->query_weight();
      if (wt >= 20)
        write("It looks very heavy.\n");
      else if (wt >= 16)
        write("It looks heavy.\n");
      else if (wt >=8 && wt <12)
        write("It looks light.\n");
    }
    if(living(ob)) {
      if(ob != this_object()) {
       if(!query_invis()){
     // tell_room(environment(this_object()),
     //   sprintf("%s looks at %s.\n", query_name(), ob->query_name()),
     //    ({ this_object(), ob }));
     //  tell_object(ob, sprintf("%s looks at you.\n", query_name()));
          }
       }

    ho = (string)ob->query_var("sight");
    if(ho) write(ob->query_name()+" has glowing eyes.\n");
    ho = (string)ob->query_var("fangs");
    if(ho) write(ob->query_name()+" has pointy fangs.\n");
    ho = (string)ob->query_var("wings");
    if(ho) write(ob->query_name()+" has huge wings.\n");
    ho = (string)ob->query_var("claws");
    if(ho) write(ob->query_name()+" has sharp claws.\n");
    ho = (string)ob->query_var("exoskin");
    if(ho) write(ob->query_name()+" has hardened skin.\n");


    printf("%s",ob->show_eq());
    }
    else show_inventory(ob);
  }
  else if(!living(ob)) {
      wt = (int)ob->query_weight();
      if (wt >= 20)
        write("It looks very heavy.\n");
      else if (wt >= 16)
        write("It looks heavy.\n");
      else if (wt >=8 && wt <12)
        write("It looks light.\n");
      else if (wt <8)
        write("It looks very light.\n");
  }
  return 1;
}

/**************************************************************************/
/* help fun*/

status help(string what) {  
    string tmp1, tmp2; 
    string pre,rest;  
    int i;

    if(!what 
    || sscanf(what,"%s/%s",pre,rest) 
    || sscanf(what,"%s.%s",pre,rest)) {  
      what = "help";
    }
    what = subst(what," ","_");
    if(file_size("/doc/help/"+what+"") > 0){
      more("/doc/help/"+what+"");
    return 1;
    }
  if(secure(SEC1) && file_size("/doc/mudlib/"+ what ) > 0) {
      more("/doc/mudlib/"+ what );
    return 1;
    }
    return notify_fail(sprintf("No help on %O\n",what)),0;
}

/***************************************************************************/
/* kill */

static status kill(string str) { 
  object ob; 
  if(dead) return 0; 
  if(!str) { 
    write("Kill what ?\n"); 
    return 1; 
  } 
     if(query_position() == "sleeping"){
     write(GREY_F"You are sleeping!\n"NORM);
     return 1; }
   if(!(ob = present(lower_case(str), environment()))) {
    write("No " + str + " here !\n"); 
    return 1; 
  } 
  if(!living(ob)) { 
    write(ob->short() + " is not a living thing !\n"); 
    say(query_name() + " tries foolishly to attack " + str + ".\n"); 
    return 1; 
  }
  if(ob == this_object()) { 
    write("What ? Attack yourself ?\n"); 
    return 1; 
  } 
  if(environment(this_object())->query_no_fight()) {
    write("Fighting is not allowed here.\n");
    return 1;
  }
  if(primary_attack == ob) { 
    write("Yes, yes !!\n"); 
    if(this_player()->is_player() && 
       this_object()->is_player())
         log_file("YES",this_object()->query_name()+" tried to RE-engage "+
                        ob->query_name()+" "+ctime()+"\n");
    return 1; 
  } 
  if(!add_secondary_attacker(ob)) {
      // old check for pk (hehe)
  }
  if(primary_attack && present(primary_attack, environment())) {
    write("You turn to attack "+(string)ob->query_name()+".\n");
  }
  primary_attack = ob;
  return 1; 
} 

 /**************************************************************************/
 /* command filter for bin system */

status command_centre(string str) {
  string verb, tmp1, tmp2;
  int i;

  verb = query_verb();

  catch(i=(status)call_other("/secure/bin_d","bin_dir_hook",str));
  if(i) return i;
   
  if(!bad_file(verb, ({ ".", "/", }))) {
      if(FILE_SIZE("/cmds/skills/"+ verb +".c") > 0) { 
        if(call_other("cmds/skills/"+ verb, verb, str))
        return 1;
      }
   }
  
}
  

/*
if(sscanf(verb,"%s@%s", tmp1, tmp2) == 2) {
    if(!bad_file(tmp1,({ ".", "/", })) && !bad_file(tmp2, ({ ".", "/", }))) {
      if(member_array(tmp1, classes) != -1) {
        if(FILE_SIZE("/skills/"+ tmp1 +"/"+ tmp2 +".c") > 0) {
          if(call_other("skills/"+ tmp1 +"/"+ tmp2, tmp2, str)) return 1;
        }
      }
    }
  }
  
*/


/*************************************************************************/

status log_for_players(string file, string str){
  string room_file, domain, creator_name, tmp;
  if(!file) return 0;
  if(!str) { write("Give an argument.\n");  return 1;  }
  room_file = file_name(environment());
  log_file(file,"\n"+ query_name() +" (" + room_file +")("+ctime()+"):\n");
  log_file(file, str + "\n");
  if((creator_name = creator(environment()))) {
    log_file(creator_name+".rep",
      "Report Type: "+ file +"  Room: "+ room_file +"\nTime: "+
       ctime(time()) +"\nPlayer: "+ query_name() + "\n" +
        "REPORT: "+str+"\n"); }
  write(file +" logged.\n");
 return 1;   }





     
/****************************************************************************/
/* whisper */

static status whisper(string str){  
    object ob;  
    string who, msg;  

    if(!str || !sscanf(str, "%s %s", who, msg)) {  
      write("Usage: whisper <who> <msg>\n");  
      return 1;  
    }  
    ob = find_living(lower_case(who));  
    if(!ob || !present(ob, environment())) {  
      write("There is no "+who+" here.\n");  
      return 1;  
    }  
    if(!this_object()->query_security())
    if(ob->query_invis()){
        write("There is no "+who+" here.\n");
     return 1;
}
      msg += NORM; 
    tell_object(ob, do_cc("")+query_alt_name() +
      " whispers to you: "+ msg+"\n"+NORM);  
    write(do_cc("")+"You whisper: "+msg+"\n"+NORM);  
    say(do_cc("")+query_alt_name() +
      " whispers something to "+ capitalize(who)+".\n"+NORM, ob); 
    return 1;  
}  


     

status save_character() {  
#ifdef SAVES_DISABLED
  tell_object(this_object(), 
     "Character saving disabled... Character not saved.\n");
#else
  tell_object(this_object(),
      "Saved\n");  
#endif
    save_me(0);  
    return 1;  
}  


/**************************************************************************/
/*                  Mud Logon - calls logon() first                       */
/**************************************************************************/

static void time_out() {  
    write("Time out\n");  
    set_heart_beat(0);
    destruct(this_object());  
}  
// start logon 

static status logon() {
  write("Accepting connection: "
   +query_ip_number(this_player())+" at "+ctime()+"\n");
  cat("/doc/help/news/welcome");
#ifdef LOCK_OUT
    write("MUD IS CURRENTLY LOCKED FROM MORTAL CONNECTIONS.\n");
#endif /* LOCK_OUT */
    write("What is your name? ");
    input_to("logon20");  
    call_out("time_out", 40);  
    return 1;  
}  

#ifdef LOCK_OUT

void destruct_lock_out() {
  destruct(this_object());
}

#endif /* LOCK_OUT */


static void logon20(string str) { 
    int i, length; 
    if(!str || str == "") { 
      remove_call_out("time_out"); 
      set_heart_beat(0);
      destruct(this_object());  
      return;  
    } 
 log_file("PING",
    "PING "+str+" "+query_ip_name()+" "+ctime()+"\n");
    str = lower_case(str);  
    if(str == "logon") {  
      write("Invalid name.\n");  
      remove_call_out("time_out"); 
      logon();
      return;  
    }  
   
    if((length = strlen(str)) > MAX_CHAR_NAME) {  
      write("Name is too long (max. "+MAX_CHAR_NAME+" characters).\n");  
      remove_call_out("time_out"); 
      logon();
      return;  
    }  
    if((length = strlen(str)) < 3){
       write("Name is too short minimum 3 characters.\n");
      remove_call_out("time_out");
      logon();
       return;
    }
    for(i = 0; i < length; i++) {  
      if(str[i] < 'a' || str[i] > 'z') {  
        write("Invalid characters in name.\n");  
        remove_call_out("time_out"); 
        logon();
        return;  
      }  
    }  
   if(VALID->test_site_wiz(str) && query_mud_port() == 8888){
        write("This name belongs to a wizard.\n");
        remove_call_out("time_out"); 
        logon();
        return;  
   }
   if((FILE_SIZE("usr/banish/"+str+".o") > 0 ||
       FILE_SIZE("usr/suicide/"+str+".o") > 0)){
      write("That name is reserved.\n");  
      remove_call_out("time_out"); 
      logon();
      return;  
    }
    name = str;
    if(!restore_me() && name != "guest") { 
        if("/cmds/admin/nameban"->name_is_banned(str)) {
            write("That name is reserved.\n");
            remove_call_out("time_out");
            logon();
            return;
    }
 write("\nCreating new character...\n"+
       "\nWARNING: new users are responsible for reading help rules.\n"+
       "Welcome "+ capitalize(name) +" !!\n");  
    } 

 if(!member)
    if(sizeof(users()) >= 
    MAX_USERS+sizeof(filter_array(users(),"filter_wizards",this_object()))) {
      if(!secure(SEC1)) {
        write("Sorry, "+ MUD_NAME +" is currently full.\n"+
              "Perhaps someone might log out soon?\n");
        remove_call_out("time_out"); 
        destruct(this_object());
        return;
      }
      else {
        write(MUD_NAME +" is at Maximum Players.\nCreators accepted.\n");
      }
    }
       
/*
    if(level < 2){
      remove_call_out("time_out"); 
      cat("/doc/help/news/new_lock");
      remove_call_out("time_out"); 
      destruct(this_object());
      return;
    }
*/
   
#ifdef LOCK_OUT
#ifdef ONLY_ALLOW
   if(member_array(name,ONLY_ALLOW) == -1)
#else
#endif // ONLY_ALLOW
    if(security_level < LOCK_OUT && security_level < SEC9) {
      remove_call_out("time_out"); 
      cat("/doc/help/news/closed");
      call_out("destruct_lock_out",5);
      return;
    }

    write("Name accepted.\n");

#endif /* LOCK_OUT */
    if(name == "guest") {
      int guest_num;

      for(i = 0; i < sizeof(users()); i++) {
        if((string)users()[i]->query_name(1) == "guest") guest_num += 1;
      }
      if(guest_num > MAX_NUM_GUESTS) {
        write("Sorry, there are too many Guests.\n");
        remove_call_out("time_out");
        set_heart_beat(0);
        destruct(this_object());
        return;
      }
      remove_call_out("time_out");
      accept_connection(#'logon70);
      return;
    }
    other_copy = find_player(name);
    write("What is your password? ");
    input_to("check_password", 1);  
    return;  
} 

static void check_password(string p) { 
  write("\n");
  if(!other_copy) restore_me();
  if(++password_check > PASSWORD_CHECK) {
    write("\nThat was your last try!\n");
    remove_call_out("time_out"); 
    destruct(this_object()); 
    return;
  }
  if(!p || p == "") {
    write("What is your password?? ");
    input_to("check_password",1);
    return;
  }
  if(!password) {
    new_password(p);
    return;
  }
  if(crypt(p, password) != password) { 
 log_file("LOGFAILS",
    "FAILED ON "+query_name()+" "+query_ip_name()+" "+ctime()+"\n"); 
     if(!PASSWORD_CHECK-password_check){ 
       remove_call_out("time_out"); destruct(this_object()); }
     write("\nWrong password!"+
          " (You have "+(PASSWORD_CHECK-password_check)+" attempts left) > ");
    input_to("check_password",1);
    return;
  }
  remove_call_out("time_out"); 
  logon30("");
} 

static void new_password(string p){ 
  write("\n"); 
  if(!p || p == "") {
    write("What will your password be? ");
    input_to("check_password",1);
    return;
  }
  if(strlen(p) < 6) { 
    write("The password must have at least 6 characters.\n"); 
    input_to("new_password", 1); 
    write("What will your password be? "); 
    return; 
  } 
  if(!password) { 
    password = p; 
    input_to("new_password", 1); 
    write("What was the password again? ");
    return; 
  } 
  remove_call_out("time_out"); 
  if(password != p) { 
    write("You changed! Try again.\n");
    destruct(this_object()); 
    return; 
  } 
  password = crypt(password,0);    /* Generate new seed. */ 
  save_me(0);
  logon30("");
} 


static int email_count = 0;
 
static void logon30(string str){
   string reason;
   reason = 0;
  if(!email || email == "") {
      reason = "A valid email is required.\n";
   } else {
if(!member(email,'@')) {
         reason = "A valid email is required.\n";
      } else {
         string user, host, *p;
         p = explode(email,"@");
         if(sizeof(p) != 2) {
            reason = "Your email address needs to be of the format 'user@host'.";
         } else {
            int i;
            user = p[0];
            host = p[1];
            if((user == "none")
                   || (user == "user")
            || (user == "blah")
                       ) {
               reason = "That domain is invalid.\n";
            } else {
               if((i = sizeof(p = explode(host,"."))) < 2) {
                  reason = "Your email address contains an invalid hostname.";
               } else {
                  if((strlen(p[<1]) > 3) || (strlen(p[<1]) < 2)) {
                     reason = "Boy that's really creative.  Care to try again?\nThis time try a real hostname.";
                  }
               }
            }
         }
      }
   }
   if(reason && (email != "npemail")) {
      if(email_count++ > 2) {
         email_count = 0;
     printf("\nAn email is required to be verified as an actual player.\n");
      }
      printf("%s\nPlease enter your email address:  ",reason);
      input_to("logon40");
      return;
   }
  if(query_ip_name() != called_from_ip && called_from_ip)  
  write("Your last login was from " + called_from_ip + "\n");  
  if(last_on) {
    write("Your last login was on "+ ctime(last_on) +".\n");  
  }
  ip_number = query_ip_number();
  called_from_ip = query_ip_name();  
  do_log_me("LOGON");
  accept_connection(#'logon50);
} 
 
void logon40(string e) {
   email = e;
   logon30("");
   return;
}

static void logon50(string str){
   if(last_off < 982210000){
      last_off = time();
      set_race("unknown");
      set_profession("adventurer");
      classes = ([]);
      set_all_stats(25);
      set_all_maxstats(75);
   }

   this_object()->do_update(); // must be here..
   last_on = time(); // this is vital to do_update
   if(!(gender == 1 || gender == 2)) { 
    write("\nIs your character [m]ale or [f]emale?\n> "); 
    input_to("logon53"); 
    return; 
  } 
   logon70("");
} 


static void logon53(string gender_str) { 
  gender_str = lower_case(gender_str); 
  if(gender_str[0] == 'm') { 
    write("\nWelcome, Sir!\n"); 
    gender = 1;
  } 
  else if (gender_str[0] == 'f') { 
    write("\nWelcome, Madam!\n"); 
    gender = 2;
  } 
  else { 
    write("\nAre you, [m]ale or [f]emale? "); 
    input_to("logon50"); 
    return; 
  }   
  logon70(""); 
} 

// color question

static void logon60(string str){
  if(!query_env_var("ANSI")){ 
      write("\nDo you wish to have full color? [y]es or [n]o?\n> ");
      input_to("logon63");
      return;
  }
  logon70("");
}

void logon63(string str){
   if(str == "y" || str == "yes"){
      this_object()->set_env_var("ANSI",1);
      logon70(""); 
      return;
   }
      this_object()->set_env_var("ANSI",0);
      logon70("");
      return;
}


// name question 

static void logon70(string str){
  if(!create){ 
      write("\n"PRINTBAR"\n");
      cat("/doc/help/policy/names");
      write("Do you meet these requirements? [y]es or [n]o?\n> ");
      input_to("logon72");
      return;
  }
  logon80("");
}

void logon72(string str){
   if(str == "y" || str == "yes"){
      create = time();
      logon80(""); 
      return;
   }
   printf("\nWhat would you like for a name?\n");
   input_to("logon20");
   return;
}


// START ALIGNMENT

/*

static void logon75(string str) {
  if((int)VALID->is_valid_alignment(alignment) != 1) {
  write("\n"PRINTBAR"\n");
   cat("/doc/help/theme/alignments");
  write("What is your alignment?\n> ");
  input_to("logon77");
  return;
     }
  logon80("");
  return;
}


static void logon77(string str) {
  if(!str) str = "";
  if( (int)VALID->is_valid_alignment(str) != 1){
    logon75("");
    return;
  }
  str = lower_case(str);
  this_object()->set_alignment(str);
  write("\n"PRINTBAR"\n");
  cat("/doc/help/etc/"+str);
  write("Are you sure you want this alignment? [y]es or [n]o?\n> ");
  input_to("logon79");
  return;
}

void logon79(string str){
   if(str != "y" && str != "yes"){
   set_alignment("remove");
   logon75("");
   return;
   }
   else {
   logon80("");
   return;
   }
}
*/

// START RACE

static void logon80(string str) {
  if(!this_player()->query_security_level()) 
  if(!(int)VALID->is_valid_race(race)){
   write("\n"PRINTBAR"\n");
   set_all_stats(25);
   cat("/doc/help/theme/races");
  write("What is your race?\n");
  input_to("logon81");
  return;
     }
  logon85("");
  return;
}


static void logon81(string str) {
  if(!str) str = "";
  if(!(int)VALID->is_valid_race(str)){
    logon80("");
    return;
  }
  str = lower_case(str);
  if(str == "dragonkin") logon80("");
  this_object()->set_race(str);
  enter_room = 0;
  init_stats(race,profession);
  write("\n"PRINTBAR"\n");
  cat("/doc/help/etc/"+str);
  write("Are you sure you want this race? [y]es or [n]o?\n> ");
  input_to("logon82");
  return;
}

void logon82(string str){
   if(str != "y" && str != "yes"){
   set_race("remove");
   logon80("");
   return;
   }
   else {
   logon85("");
   return;
   }
}
// START CLASS

static void logon85(string str) {
  if((int)VALID->is_valid_profession(profession) != 1) {
     write("\n"PRINTBAR"\n");
     cat("/doc/help/theme/classes");
  this_object()->unset_all_classes();
  write("What is your class?\n> ");
  input_to("logon86");
  return;
     }
  logon90("");
  return;
}


static void logon86(string str) {
  if(!str) str = "";
  if((int)VALID->is_valid_profession(str) != 1) {
    logon85("");
    return;
  }
  str = lower_case(str);
  this_object()->set_profession(str);
  write("\n"PRINTBAR"\n");
  cat("/doc/help/etc/"+str);
  write("Are you sure you want this class? [y]es or [n]o\n> ");
  input_to("logon87");
  return;
}

void logon87(string str){
   if(str != "y" && str != "yes"){
   set_profession("remove");
   logon85("");
   return;
   }
   else {
   logon90("");
   return;
   }
}

// START LOG IN

static void logon90(string str) {
  write("\n"PRINTBAR"\n");
  if(age < 3600)
   log_file("NEWBIE",
    capitalize(name)+"::"+ctime()+"::"+query_ip()+"\n");
 if(secure(SEC1)) {
   other_copy = this_object();
    efun::clone_object(WIZARD);
     return;
  }
   logon100("");
   return;
}

status query_entering() { return entering; }

static void move_enter(object where) { 
    int in_prison;
    entering = 1;
    if(objectp(where)) {
    tell_room(where, YELLOW_F+query_name() +" arrives suddenly!\n"+NORM);
    move_object(this_object(),where);
    entering = 0;
    look(0,0);
    return;
  }
  if(enter_room){
    move_player("X#"+enter_room);  
    entering = 0;  
  }
  if(!enter_room){
    church();
  }
}

static void logon100(string str) { 
  object ob; 
  object where;
  string tmp_name; 
  status new_mail_flag;
  tmp_name = name;  
  name = 0;  
  other_copy = find_player(tmp_name);  
  name = tmp_name;  
       if(other_copy){
            if(query_ip_number(other_copy)) 
            write("Your character seems to already be in the game !\n");  
                exec(other_copy,this_object());
                  other_copy->reconnect();
      tell_object(this_object(),
      sprintf("\nSomeone from %s has taken over your player.\n",
      query_ip_name(other_copy)));
                destruct(this_object());
                return;
        }
    write(NORM);


     unset_attrib("remove");
  if(query_attrib("silenced") < time())
     unset_attrib("silenced");
     unset_attrib("silenced_by");
  if(query_attrib("disabled") < time())
     unset_attrib("disabled");
     unset_attrib("disabled_by");
  if(query_attrib("silenced")){
   write("You are currently silenced. Mail dh@dh.mudservices.com.\n");
   write("if you have any questions regarding your condition.\n");
  }
  if(query_attrib("disabled")){
   write("You are currently disabled. Mail dh@dh.mudservices.com,\n");
   write("if you have any questions regarding your condition.\n");
   this_object()->quit_2();
  }

  if(!query_attrib("disabled")){
  enable_commands();  
  set_living_name(lower_case(name));
  time_to_save = age + 300; // do next save
  alias_name = capitalize(name);  
  add_standard_commands();  
  }  
 if(member){ 
    cat("/doc/help/news/member");
}
 
 cat("/doc/help/news/news");
  if(secure(SEC1)) cat("doc/help/mudlib/wiznews");
  if(catch(move_enter(where))) {
    write("Entry Room Failure!\n");
    church();
  }

  this_object()->init_link(query_pretitle());

  if(catch(restore_inventory(this_object()))) {
    write("Inventory Restoring Failure!\n");
  }
  if(catch(do_storage_backup())) {
    write("WARNING! Failed to Backup Inventory info. Type 'save' now.\n");
  }

  entering = 0; /* failsafe */

  if(security_level > SEC3) is_invis = 1;
  if(is_invis)              write("\nYOU ARE INVISIBLE !\n\n");  

  if(create < 815162401 && !security_level) create=time();
 
  save_me(0);
  set_heart_beat(1);
  if(level == -1) { /* new player */
     enter_room = VOID;
     level = 1;
     create = time();
     money = 0;
     adj_hp(200);
   } 
   recalc_ac();
   recalc_wc();
   recalc_carry();

   catch(save_me(1));
   catch(SHUTD->shutdown_warning());  // Do not remove
   catch(tmp_name = (string) MAILD->query_new_mail(name,1));
   if(tmp_name) printf("You have %s at the post office.\n",tmp_name[0..<3]);
   if(::defined_aliases("AUTOSTART"))
    
   call_out("init_command",2,"AUTOSTART");
   call_out("update_player_ranking",0);
   call_out("restore_eq",0);
   update_master_exp();
}  


static void logon95(string str) {  
    string fname1, fname2;
    object where;  
    int i;
 
    if(str == "" || (str[0] != 'y' && str[0] != 'Y')) {  
        write("Welcome another time then !!\n"); 
        destruct(this_object());  
        return;  
    }  
  
    if(other_copy) {
        other_copy->save_me(0);
        enable_commands();
        this_object()->init_prompt();
        this_object()->init_link(query_pretitle());
        sscanf(file_name(this_object()),"%s#%d",fname1,i);
        sscanf(file_name(other_copy),"%s#%d",fname2,i);
        where = environment(other_copy);
        if(fname1 == fname2) {
            tell_object(other_copy,
               sprintf("Someone at %O logged in as you and threw you out.\n",
                       (query_ip_name(other_copy))));
            other_copy->quit_2(0);
        } else {
          destruct(other_copy); }
    }
    if(restore_me() && fname1 == fname2) {
        write("Restoring points from other copy...\n");
    } else {
        if(security_level) {
            write("Granting Creator Powers.\n");
        }
    }
    logon100("");
}  


int query_last_save() { return last_save; }

int query_no_save(){  // used for external queries
#ifdef SAVES_DISABLED
   return 1;
#endif
   return 0;
}
 
// func save_me save func

void save_me(status value_items) {  
#ifdef SAVES_DISABLED
    return;
#endif
    if(this_object()->query_attrib("saveDisable") && query_verb() != "quit")
        return;
    this_object()->set_start();
    if(environment()) do_storage_backup();
    this_object()->save_eq();
    if((int)MASTER->master_file_size(SAVE_NO_BANISH+query_name(1)+".o")>0){
        save_object(PSF = SAVE_NO_BANISH+ query_name(1));
    } else if(security_level) {
        save_object(PSF = SAVE_WIZARD+lower_case(name));
        if(SAVE_WIZARD != SAVE_FIRST) {
            MASTER->master_remove_file("/"+SAVE_FIRST+query_name(1)+".o");
        }
        if(SAVE_WIZARD != SAVE_PLAYER) {
            MASTER->master_remove_file(GET_SAVE_PLAYER(query_name(1)));
        }
    } else if(age < 43200) {
        save_object(PSF = SAVE_FIRST+lower_case(name));
        if(SAVE_FIRST != SAVE_PLAYER) {
            MASTER->master_remove_file(GET_SAVE_PLAYER(query_name(1)));
        }
    } else {
        save_object(PSF = GET_SAVE_PLAYER(lower_case(name)));
        if(SAVE_PLAYER != SAVE_FIRST) {
            MASTER->master_remove_file("/"+SAVE_FIRST+query_name(1)+".o"); 
        }
    }  
  return;
}
 
/*************************************************************************/
/* restore_me() - all player restores are thru' this fn */
    
status restore_me() {
    Restored = restore_object(PSF = SAVE_NO_BANISH + lower_case(name));
    if(!Restored && restore_object(PSF = SAVE_WIZARD+lower_case(name)))
        Restored = 1;
    if(!Restored && restore_object(PSF = SAVE_FIRST+lower_case(name)))
        Restored = 1;
    if(!Restored && restore_object(PSF = GET_SAVE_PLAYER(lower_case(name))))
        Restored = 1;
//    if(!Restored && restore_object(PSF = "usr/"+lower_case(name)))
//        Restored = 1;
   if(Restored) {
        last_save = file_time(PSF+".o");
   } else {
        PSF = last_save = 0;
   }
    return Restored;
}
 
/****************************************************************************/
/* quit2  The reverse of loggin on  */
status quit_ld(mixed arg){
      if(this_object()->query_security_level()){
          return 1;
       }
       if(this_object()->query_primary_attack()){
       call_out("remove_linkdead",10);
       return 1;
       }
       if(this_object()->query_last_combat()+10 > time()){
       call_out("remove_linkdead",10);
       return 1;
       }
       quit_2(1);
    return 1;
}
status quit(mixed arg){
   if(arg) return 1;
      if(this_object()->query_last_combat()+10 > time()){
      write("Why dont you relax first?\n");
      return 1;
      }
      if(this_player()->query_primary_attack()){
      write("You have unfinished business to settle first!\n");
      return 1;
       }
     write(GREY_F);
     cat("/doc/help/news/exit");
     write(NORM);
     quit_2(1);
     return 1;
}

status quit_2(mixed arg) {
  say(capitalize(name)+ " has left the world.\n");
  last_off = time();
  do_log_me("LOGOUT");
  if(stringp(arg)) arg = 0; 
  remove_call_out("quit_2");
#ifdef SAVES_DISABLED
  write("Character saving disabled... Character not saved.\n");
#else
  write("Saving....\n");
#endif
  set_heart_beat(0);
  store_inventory();
  last_off = time();
  save_me(1);
  destruct_all_inventory(this_object());
  rm(PASTE);
  set_heart_beat(0);
  update_player_ranking();
  destruct(this_object()); 
  return 1; 
} 

/* move or destruct inventory to environment */


static void move_or_destruct_inventory(object ob) {
  int i, wt;
  object *inv, env;

  if(!(env = environment(ob))) {
    destruct_all_inventory(ob);
    return;
  }
  inv = all_inventory(ob);
  for(i = sizeof(inv); i--; ) {
    if(inv[i]) {
      if(inv[i]->drop(1)) {
        if(inv[i]) destruct(inv[i]);
      }
      else {
        if(inv[i]) {
          wt = (int)inv[i]->query_weight();
          ob->add_weight(wt*(-1));
          move_object(inv[i], env);
          env->add_weight(wt);
        }
      }
    }
  }
}


/***************************************************************************/
/* destruct inventory */

static void destruct_all_inventory(object ob) {
  int i, size;
  object *inv;

  inv = deep_inventory(ob);
  for(i = 0, size = sizeof(inv); i < size; i++) {
//     if(inv[i]) destruct_all_inventory(inv[i]);
    if(inv[i]) destruct(inv[i]);
  }
}

 
/****************************************************************************/
/* renews player object */

void get_new_player_object() {  
  int i;
  string fname;
  save_me(1);
  sscanf(file_name(this_object()),"%s#%d",fname,i);   
  if(!(fname == WIZARD || fname == PLAYER)) return;
  other_copy = this_object();
  if(security_level)
      efun::clone_object(WIZARD);
  else
      efun::clone_object(PLAYER);  
}  


/**********************************************************************/
/* make_path - for players */

string make_path(string file) {
  string tmp1, tmp2;
  string *path_split;
  int i;

  if(sscanf(file,"%s..%s", tmp1, tmp2)) {
    path_split = explode(file,"/");
    while((i = member_array("..",path_split)) != -1) {
      path_split = path_split[0..i-2]+path_split[i+1..sizeof(path_split)-1];
    }
    file = "/" +implode(path_split,"/");
    while(sscanf(file, "%s//%s", tmp1, tmp2)) file = tmp1 + "/" + tmp2;
  }
  return file;
}


     
/*****************************************************************************/
/* A NO valid_read() restricted file_size */

static int FILE_SIZE(string file) {
  return (int)MASTER->master_file_size(file); 
}

static status more(string file) {
  "cmds/wiz/more"->main(file);
  return 1;
}

status cat(string path){  
  "cmds/wiz/cat"->main(path);
  return 1;  
}  


void toggle_last_combat()  { last_combat = time(); }
void toggle_last_pk(){ last_pk = time(); }
void toggle_last_death()  { last_death = time(); }
int  query_last_combat(){ return last_combat; }
int  query_last_death(){ return last_death; }
int  query_last_pk(){ return last_pk; }

status toggle_death() {  
    tell_object(this_object(),  
      "You feel a very strong force pulling your body back to reality...\n"+ 
      "Your body solidifies in a more solid form!\n");  
    say(query_name() + "'s body begins to solidify...\n"+  
        query_name()+" appears whole once more!\n");  
    dead  = 0;  
  this_object()->church();
  save_me(0);  
  return 1;  
}  


/****************************************************************************/
/* security level routines */

nomask status security(int sec_level){ 
  if(!query_ip_number(this_player())) return 0;
    return ((int)this_player()->query_security() >= sec_level
           || this_player() == this_object()) ? 1 : 0;
} 


nomask status secure(int sec_level) {
  object user;

  if(!(user = (this_interactive()) ? this_interactive() : this_player())) {
    return 0;
  }
  if(!query_ip_number(user)) return 0;
  return ((int)user->query_security() >= sec_level) ? 1 : 0;
}

nomask int query_security() { 
  return security_level; 
} 

nomask int query_security_level() {
  return security_level;
} 



nomask int set_security_level(int new_level) {
  string str;
  int old_level;
  object wiz_creator;

  old_level = security_level;

  if(!this_player() || !interactive(this_player())
  || previous_object() != this_player()) {
    write("Use the 'promote' command.\n");
    return 0;
  }
  if(secure(SEC9)) {
    if(security_level < SEC9 || new_level > security_level) {
      security_level = new_level > 100 ? 100 : new_level;            
    }
  }
  else if(secure(SEC9) && new_level < SEC8) {
    security_level = new_level;
  }
  if(old_level == security_level) {
    write("It is not possible for you to promote "+ query_cap_name()
         +" to a higher security level.\n");
     return security_level;
   }
   tell_object(this_object(),"Welcome to the next level.\n");
   create_wizard(query_name(1)); 
   this_object()->set_balance(0);
   save_character(); 
   write("You have promoted "+ query_name(1) +" to "+ new_level +".\n"); 
   write("Security Level: "+ security_level +"\n"); 

   log("LOG_SPONSER","Lvl: "+ level +", Sec Lvl: "+ old_level,
                   "Sec Level: "+ security_level);
    
    get_new_player_object(); 
    return security_level; 
} 



int set_savings(int s) { 
  log_file("/MAIN/BANK",this_player()->query_real_name()+ " set_savings "+
  "from "+savings+" to "+(savings+s)+" on "+time()+"\n");
  return savings = s; }

int add_savings(int s) {
  log_file("/MAIN/BANK",this_player()->query_real_name()+ " add_savings "+
  "from "+savings+" to "+(savings+s)+" on "+time()+"\n");
  return savings += s; }

int add_exp(int e){
    int max_level_xp;
    max_level_xp = level * MAX_XP_PER_LVL;
    if(e > max_level_xp) e = max_level_xp;
 //  e = (e * (query_max_hp() - whimpy))/query_max_hp();
    if(e > 0) {
        if(total_exp + e < total_exp) // It rolled over...
            total_exp = (1 << 31) - 1; // Set to MAXINT
        else
            total_exp += e;
    }

    update_master_exp();
    
    return ::add_exp(e);
}

int add_intoxication(int i) { 
  return intoxicated = (intoxicated + i < 0) ? intoxicated + i : 0; 
}

int add_stuffed(int i) {
  return stuffed = (stuffed + i < 0) ? stuffed + i : 0;
}

int add_soaked(int i) {
  return soaked = (soaked + i < 0) ? soaked + i : 0;
}



/************************************************************************/
/* report to check for cheating.. */
void do_log_me(string sitc){
  string log_str, s1, s2, s3, site_str;
         log_str = "["+name+" L:"+level+" S:"+savings+"/ M:"+money+  
           " IP:"+called_from_ip+" T:"+time()+"]\n"; 
         sscanf(called_from_ip,"%s.%s",s1,s2);
//    log_file(sitc+"/"+sitc,log_str);
    log_file(sitc,log_str);
/*
  log_file(sitc+"/"+called_from_ip, log_str);
*/
  }
/************************************************************************/
/* scars */

static void make_scar() { 
  scar |= 1 << random(10); 
} 
void remove_scar() { 
  scar = 0; 
} 

void show_scar() { 
  int i, j, first, old_value; 
  string *scar_desc; 

  scar_desc = ({ 
                "left leg", "right leg", "nose", "left arm", "right arm", 
                "left hand", "right hand", "forehead", "left cheek", 
                "right cheek",
              }); 
  old_value = scar; 
  for(j = 1,first = 1; i < 10;j *= 2,i++) { 
    if(scar & j) { 
      old_value &= ~j; 
      if(first) { 
        write(query_name() +" has a scar on "+ query_possessive() + 
              " " + scar_desc[i]); 
        first = 0; 
      }
      else if(old_value) { 
        write(", " + query_possessive() + " " + scar_desc[i]); 
      }
      else { 
        write(" and " + query_possessive() + " " + scar_desc[i]); 
      } 
    } 
  } 
  if(!first) write(".\n"); 
} 



     
/**************************************************************************/

static int *last_death_undo_buffer = 0;

string undo_last_death() {
    if(!last_death_undo_buffer
    || !sizeof(last_death_undo_buffer))
        return "no death to restore";
    if(!this_interactive() 
    || this_interactive()->query_security_level() < 100)
        return "no security privilege";
    experience += last_death_undo_buffer[0];
    level = last_death_undo_buffer[1];
    last_death_undo_buffer = 0;
    tell_object(this_object(),"\n\nYou have been granted the experience and level lost by your last death.\nYou must regain your skills and repray for stats yourself.\n\n");
    tell_object(this_interactive(),"\n\nYou have restored the death of " + capitalize(name) + "\n\n");
    return "death restored";
}    
    
status death_2() {
 if(dead)
    log_file("GHOSTS", name+" "+level+
       ", KLR:"+(string)this_player()->query_name(1)+
       ", CRE:"+creator(this_player()) + "\n"+
       ", LOC:"+query_current_room()+
       ", POB:"+ file_name(previous_object()) +"\n");
    log_file("KILLS", name+" "+level+
       ", KLR:"+(string)this_player()->query_name(1)+
       ", CRE:"+creator(this_player()) + "\n"+
       ", LOC:"+query_current_room()+
       ", POB:"+ file_name(previous_object()) +"\n");
 if(this_player() != this_object())
  if(this_object()->query_level() > 1){
   if(this_player()->is_player()){
    if(!this_object()->query_wp()) log_file("DEATHS.dat", "*");
    log_file("DEATHS.dat", this_object()->query_alt_name()+
      " ["+this_object()->query_pretitle()+"] "+
      " ("+this_object()->query_wp()+") "+
      " by "+this_player()->query_alt_name()+
      " ["+this_player()->query_pretitle()+"] "+
      " ("+this_player()->query_wp()+") "+
      extract(ctime(),0,10)+
      "\n");
    }
  }
  this_object()->unset_all_vars();
  this_object()->church();
  deaths += 1;
  dead = 1;
  toggle_last_death();

     this_object()->reduce_stats_by_one();
     this_object()->reduce_skills_by_one();
     level = level -1;  

  make_scar();  
  headache    = 0;  
  intoxicated = 0;  
  stuffed     = 0;  
  soaked      = 0;
  hp = 100;  
  primary_attack = 0;
  secondary_attacks = ({});
   tell_object(this_object(),
     "\n You slump to the ground in agony!      \n"+
     "              YOU DIE...                  \n"+  
     "  Your soul begins to leave your body...  \n"+  
     "You have a strange feeling as you look on \n"+  
     "     your own dead form from above.       \n\n");  
  recalc_wc();
  recalc_ac();
  recalc_carry();
  save_me(0);
  return 1;  
}  

     
string query_ip() {if(!called_from_ip) return "NONE"; return called_from_ip; }

string query_creation() { if(!create) create = time();
                          return ctime(create); }
int query_create() { if(!create) create = time();
                          return (create); }


varargs
status move_player(string dir_dest,mixed optional_dest_ob,status safe) {
  this_object()->unset_tmp_attrib("PlayerStationary");
  set_this_player(this_object());
  return ::move_player(dir_dest,optional_dest_ob,safe);
}

void notify_destruct() {
    if(previous_object() == find_object(__MASTER_OBJECT__))
        destruct_all_inventory(this_object());

}


status emote_hook(string arg) {
   status ret;
   catch(ret = (status) EMOTE_D->emote_hook(arg));
   return ret;
}

// do_update func

int do_update(){
  int i,t,d;
  if(last_update > LAST_UPDATE) return 1;   // pc updated already
  if(create > LAST_UPDATE) return 1;        // pc made after update
            total_exp = 0;
            master_exp = 0;
           // update_master_exp();
            set_profession("adventurer");
            level = 1;
            wp = experience = money = savings = 0;
            set_race("");
            house = 0;
            quests = ({});
            inventory = ({});
            backup_inv = ({});
            stats = ([]);
            classes = ([]);
            skills = ([]);
            enter_room = VOID;
            varibles = ([]);
            last_update = time();


}


/************************************************************************/


int drink_alcohol(int drink_strength) {  
  string tmp1, tmp2;
  int drink_bonus;

  if(intoxicated + drink_strength - drink_bonus >
       query_stat("stamina") *4){
   tell_object(this_object(),"You fail to reach the drink with your mouth.\n");
   return 0;  
  }  
  intoxicated += drink_strength/2;  
  if(intoxicated < 0) intoxicated = 0;  
  if(!intoxicated && previous_object()
  && (string)previous_object()->query_object_type() != "Potion") {
    tell_object(this_object(),"You are completely sober.\n");
  }  
  else if(headache) {  
    headache = 0;  
    tell_object(this_object(), "Your headache disappears.\n");  
  }  
  if(intoxicated > max_headache) max_headache = intoxicated;  
  if (max_headache > 8) max_headache = 8;  
  return 1;  
}  

     
/**************************************************************************/
/* drink soft */

status drink_soft(int drink_strength) {  
   int drink_bonus;
   if(soaked + drink_strength - drink_bonus >
       query_stat("stamina") *4){
    tell_object(this_object(),
                "You can't possibly drink that much right now!\n" +   
                "You feel crosslegged enough as it is.\n");  
      return 0;  
  }  
  soaked += drink_strength * 2;  
  if(soaked < 0) soaked = 0;  
  if(!soaked && previous_object()
  && (string)previous_object()->query_object_type() != "Potion") {
    tell_object(this_object(),"You feel a bit thirsty.\n");
  }
  return 1;  
}  


     
/**************************************************************************/
/* eat food */

status eat_food(int food_strength) {  
  if(stuffed + food_strength > query_stat("stamina") * 4){
    tell_object(this_object(),
    "This is much too rich for you right now! Perhaps something lighter?\n"); 
    return 0;  
  }  
  stuffed += food_strength * 2;  
  if(stuffed < 0) stuffed = 0;  
  if(!stuffed)  
    tell_object(this_object(),"Your stomach makes a rumbling sound.\n");  
  return 1;  
}  




/********************************************************************/
// Quest Code!

int unset_all_quests() { 
        quests = ({ }), quest_points = 0;
        return 1;   }
status query_quests(string what) {
       if(!quests || !pointerp(quests)) quests = ({});
        return (member_array(what, quests) == -1) ? 0 : 1;  }

string *query_done_quests() {
  if(!quests) quests = ({});  return quests; }

int query_number_done_quests() {
    return sizeof(query_done_quests()); }

string set_quest(string quest_id){
     if(!quest_id) quest_id = "None";
     if(current_quest = quest_id) return 0;
     write_file("log/QUEST/set", name+" start quest "+quest_id+
     ", on "+ctime(time())+" "+(string)previous_object()+"\n");
      current_quest = quest_id;   }

status finish_quest(string quest_id, int qp) {
   object called_by;
   if(query_no_save()) return 0;
   if(quest_id == current_quest) {
   if(member_array(quest_id, quests) == -1) {
      called_by = previous_object();
      quests += ({ quest_id, });
      quest_points += qp;
      write_file("/log/MAIN/QUEST",
      name+" finished "+current_quest+" ("+qp+") "+ctime()+
      " "+(string)called_by+"\n");
      current_quest = "None";
      return 1;  /* success! quest is now finished! */
    }
  return 0;      /* failure to finish quest */
  }
}

string set_quest_hint(string str){ hint = str; return hint; }
string quest_hint() {
    if(hint) return hint;
    if(!current_quest || current_quest == "None") return "Find a quest";
    return "No hints for this quest are available";
}

int query_quest_points(){ return quest_points; }
string *query_all_quests()     { return quests;        }
string query_current_quest() { if(!current_quest) return "None";
                               return current_quest; }


/************************************************************************/



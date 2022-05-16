////////////////////////////////////////////////////////////
// MONSTER.C          // DRAGONHEART // ASH.5.17.97       //
////////////////////////////////////////////////////////////

#include  <mudlib.h>

inherit "obj/living";
inherit   NPCSOUL;

#include  <points.h>            /* hp/mana functions */
#include  <monster.spc>         /* specifications */
#include  <spell.h>             /* Must come after monster.spc */

#define HEALING_TIME        30  /* 30 hearbeats */ 
#define STOLEN_TIME       3600  /* 180 heart beats call reset(1) */  
#define FILE_SIZE         (int)__MASTER_OBJECT__->master_file_size

status move_at_reset;    /* True will make the monster wander at reset. */  
status aggressive;       /* True will make monster attack on sight. */  
int half_exp, no_exp;
int stolen;              /* has something been stolen */ 
int healing;             /* heart_beats since last healing */  
int spchance,spdam;             /* used for special attack */
string sptar, sproom, spimmune;  /* used for special attack */
string *chat_head;       /* list of chat strings. */  
int chat_chance;         /* chance at each heart beat to chat */
string *a_chat_head;     /* list of attack chats */  
int a_chat_chance;       /* chance at each heart beat to use attack chat */
string *l_chat_head;     /* list of chats spoken in language */
string chat_language;    /* language chats spoken in */
int l_chat_chance;       /* chance each heart beat to chat */
object steal_ob;         /* alternate/additional steal fn */
object charmed_ob;
object init_ob;  
object heart_ob;  
string *helpers;
int random_pick;        /* Chance to pick up items every heart_beat. */  
int new_spell_chance;   /* chance each heart beat to cast spell */
string *spells;         /* list of 'player' spell commands */ 
int spell_chance;       /* chance each heart beat to cast spell */
mixed *default_spell;   /* specialized spell of monster */
int steal_chance;       /* chance per init to steal */
status no_kill_flag;    /* won't allow attacks on monster */
static string skill_cmd;  /* current skill command */
string *hate_attack;      /* what do will we hate */

/***************************************************************************/ 

// call_out_unique() is like call_out() but will only allow ONE call out
// per function name.  Will eventually be made an simul efun... Geewhiz
// (It is used for the wander code now)

varargs
void call_out_unique(string func, int delay, mixed a, mixed b, mixed c,
  mixed d, mixed e, mixed f, mixed g, mixed h, mixed i) { 
    if(find_call_out(func) == -1) {
    call_out(func,delay,a,b,c,d,e,f,g,h,i);
    }
    return;
}

mapping wander = 0;
varargs void set_wander(int chance, int time, string *dirs, mixed limit) {  
    if(!chance) {
        wander = 0;
        return;
    }
    wander = ([
                "time" : time,
                "chance" : chance,
             ]);

    if(wander["patrol"] = dirs)
        wander["patrol_idx"] = 0;
    
    if(limit) {
        if(intp(limit))
            wander["arealimit"] = implode(explode(file_name(this_object()),"/")[0..<3],"/");
        else if(stringp(limit))
            wander["arealimit"] = limit;
        if(stringp(wander["arealimit"]) && wander["arealimit"][0] == '/')
            wander["arealimit"] = wander["arealimit"][1..];
    }
        
    set_heart_beat(1); 
}  


void wander_around() {
    object o_room, n_room;
    string o_file, n_file;
    if(!wander || primary_attack)
        return;
    if(wander["curr_time"]++ > wander["time"]) {
        wander["curr_time"] = 0;
        if(wander["chance"] > random(100))
            if(!wander["patrol"]) {
                o_file = file_name(o_room = environment());
                random_move();
                n_file = file_name(n_room = environment());
                if(wander["arealimit"]) {
                    if(o_room == n_room)
                        return;
                    if(n_file[0..strlen(wander["arealimit"])-1] != wander["arealimit"])
                        move_player("this place",o_room);
                }
            } else {
                command(wander["patrol"][wander["patrol_idx"]++]);
                if(wander["patrol_idx"] > sizeof(wander["patrol"])) {
                   wander["patrol_idx"] = 0;
                   catch(this_object()->patrol_end());
                }
            }
    }
}



status set_hatred(string *meanies){
    hate_attack = meanies;
    return 1;
}


status query_hatred(string what){
  if(!hate_attack || !pointerp(hate_attack)) hate_attack = ({});
  return (member_array(what, hate_attack) == -1) ? 0 : 1;
  }


void attack() {
  object att;
  ::attack();
  if(!(att = query_primary_attack()) || !present(att, environment())) {
    return;
  }
  if(random(100) < spchance) {
    if(att->query_spell_immunity(spimmune)) {
      tell_object(att, query_name() +"'s attack does not affect you!\n");
    }
    else {
      tell_object(att, sptar);
      say(process_msg(sproom),({ att }) );
      att->hit_player(random(spdam));
    }
    return;
  }
}



void reset(status arg) {  
    ::reset(arg);
    if(arg) {  
        if(stolen) {
            if(steal_ob)
                steal_ob->steal(this_object());
            if(environment())
                environment()->reset(1); 
        }
        stolen = 0;
        if(move_at_reset)
                random_move();  
        return;  
    }  

    is_npc = 1;  
    set_env_var("NO_GIVE",1);
    enable_commands();  
    add_action("communicate","say");  
    add_action("skills", "", 1);
    NPCSOUL::add_feelings();
    add_action("cmd_hook","",1);
    default_spell = allocate(SPELL_ALLOCATE);
}  

int cmd_hook(string str) {
    int i;
  catch(i=(status)call_other("/secure/bin_d","bin_dir_hook",str));
  return i;
}

/*************************************************************************/
/* sets */
void set_helpers(string *str) { helpers = str; }
object set_heart_ob(object ob) { return heart_ob = ob; }  
object set_init_ob(object ob)  { return init_ob = ob;  }  
object set_steal_ob(object ob) { return steal_ob = ob; }
object set_charmed_ob(object ob) { return charmed_ob = ob; }
object query_heart_ob() { return heart_ob; }
object query_init_ob()  { return init_ob;  }  
object query_steal_ob() { return steal_ob; }
object query_charmed_ob() { return charmed_ob; }
int set_no_exp(int x) { no_exp = x; }
int set_half_exp(int x) { half_exp = x; }
int set_random_pick(int r)     { return random_pick = r; }  
int set_steal_chance(int s)    { return steal_chance = s; }
int query_steal_chance()       { return steal_chance; }
status set_move_at_reset(status i) { return move_at_reset = (i) ? 1 : 0; }  
status set_aggressive(status a)    { aggressive = a; return a; }
int query_aggressive(){ return aggressive; }
string random_name(){ return ((string)"/cmds/std/mknames"->random_name()); }

string set_name(string n) {  
    set_living_name(lower_case(n));  
    set_short(capitalize(n));
    set_alt_name(lower_case(n));
    return ::set_name(capitalize(n));
}  

/*** set level will automatically set monster to bare minimum requirements ***/ 


string query_object_type() { return "monster"; }
void set_level(int l) {
    int rand;
    int *monster_exp; 
    int bonus_levels;
    int part_exp;
    if(this_object()->query_tactic() == "unfocused"){
      this_object()->set_tactic("defensive");
    }
    if(!intp(l) || l < 1) l = 1; 
    level = l;
    bonus_levels = l;
    part_exp = l;
    set_wc(10+level);
    set_all_stats(20+level);
    set_ac(level*5);
    adj_hp(10000);
    adj_sp(10000);
    adj_mp(10000);

    if(level > 99)
    log_file("TOKEN/NPC", ":"+this_object()->query_name()+
    ", NPC "+file_name(this_object())+", "+ctime()+"\n");

    if(part_exp > 100) part_exp = 100;

    monster_exp = ({ 
      100,      200,     300,      400,    500,
      700,      900,     1100,     1300,   1500,
      1700,     1900,    2100,     2300,   2500,
      4000,     5500,    7000,     8500,   10000,
      11000,    12000,   13000,    14000,  16000,
      18000,    20000,   22000,    24000,  26000,
      30000,    32000,   34000,    36000,  38000,
      42000,    45000,   48000,    51000,  53000,
      55000,    57000,   59000,    61000,  63000,
      65000,    67500,   69000,    71500,  73000,
      75500,    78000,   81500,    83000,  85000,
      100000,   101000,  102000,   103000, 104000,
      105000,   106000,  107000,   108000, 109000,
      110000,   111000,  112000,   113000, 114000,
      115000,   116000,  117000,   118000, 119000,
      120000,   121000,  122000,   123000, 124000,
      130000,   131000,  132000,   133000, 134000,
      136000,   138000,  142000,   144000, 146000,
      150000,   154000,  158000,   162000, 166000,
      170000,   174000,  178000,   182000, 186000,
      190000, });
          experience = monster_exp[part_exp]; 
          if(bonus_levels < 100) bonus_levels = 0;
          if(bonus_levels) bonus_levels = bonus_levels-100;
          experience += (bonus_levels) * 5000;

}  

varargs query_spells() { return spells; }

varargs void add_object(string name, string path){
    if(!present(name)){
    call_out("do_add_object",0,name,path);
    return;
    }
}

varargs void do_add_object(string name, string path){
    object obj;
    if(!name || !path) return 0;
    obj = find_object(path);
    obj = clone_object(path);
    if(!obj) return 0;
   if(environment())
   if(present(name,environment())){
       obj = present(name);
     if(!obj->is_living()){
       move_object(obj,this_object());
       init_command("wear all");
     }
   }       
   if(environment())
   if(!present(name,environment())){
       move_object(obj, this_object());
       init_command("wear all");
   } 
   return 0;
} 



int query_exp(){
    int bonus;
     set_heart_beat(1);
     if(no_exp) experience = 1;
     if(half_exp) experience = experience/2;
     bonus = experience*"cmds/wiz/exex"->query_bonus()/100;
    return (int)experience+(int)bonus+((int)random(experience/10));

}



/************************************/


int    set_special_chance(int c)    { return spchance = c; }  
int    set_special_dam(int d)       { return spdam = d; }  
string set_special_mess1(string m)  { return sptar = m; }  
string set_special_mess2(string m)  { return sproom = m; }  
string set_special_type(string t)   { return spimmune = t; }  

string set_spell_skill_type(string str) { 
    string *spell_skills; 
    spell_skills = ({  }); 
    if(!str || member_array(str, spell_skills) == -1) { 
    str = spell_skills[random(sizeof(spell_skills))]; 
    } 
    return SPELL_TYPE = str; 
}  

/**************************************/
void set_spell(mixed *arr) {
    int i;

    default_spell = allocate(SPELL_ALLOCATE); 
    for(i = 0; i < sizeof(arr); i++) {
    if(!stringp(arr[i])) continue;
    switch(arr[i]) {
    case "target":                    TARGET      = arr[++i];     break;
    case "name": case "spellname":    SPELL_NAME  = arr[++i];     break; 
    case "school": case "sphere":     SPELL_TYPE  = arr[++i];     break;
    case "cost":                      SPELL_COST  = arr[++i];     break;
    case "damage":                    SPELL_DAM   = arr[++i];     break;
    case "msg target":                TARGET_MSG  = arr[++i];     break;
    case "msg room":                  ROOM_MSG    = arr[++i];     break;
    case "msg caster":                CASTER_MSG  = arr[++i];     break;
    case "area":    AREA_EFFECT = all_inventory(environment());   break;
    case "immune":                    IMMUNE_TYPE = arr[++i];     break;
    case "level": case "spell level": SPELL_LEVEL = arr[++i];     break;
    case "spell object":              SPELL_OBJ   = arr[++i];     break;
    case "time": case "cast time":    SPELL_TIME  = arr[++i];     break;
    case "casting msg":               PREPARE_MSG = arr[++i];     break;
    case "casting msg room":        PREP_MSG_ROOM = arr[++i];     break;    
    case "component":                 COMPONENT   = arr[++i];     break;
    case "passive":                   PASSIVE     = 1;            break;
    case "aggressive":                PASSIVE     = 0;            break;
    case "chance":                   spell_chance = arr[++i];     break;
    }
    }
}


/* can call player spells */

void load_spells(int c, string *s) { 
    sizeof(s); 
    spells = s; 
    spell_chance = c; 
} 


/*************************************************************************/
/* query */

int is_npc(){ return 1; }
status query_no_kill_flag() { return no_kill_flag; }
void check(){ 
    if(this_object()->query_current_room() == "NONE"){
    destruct(this_object());
    }
}




/***********************************************************************/
/* heart beat */

static status filter_players(object ob) {
    if(living(ob) && !ob->query_npc()) return 1;
    return 0;
}


int helped = 0;

void heart_beat() {  
    int player_here;
    string cmd;
    object *inv, *players;

    update_timed();
    if(!environment()) {
        set_heart_beat(0);
        return;
    }

    if(!primary_attack) {
        helped = 0;
    }

    age += 1;  

    if(heart_ob)
        heart_ob->monster_heart_beat();  

    this_object()->update_timered();

    inv = all_inventory(environment());
    player_here = sizeof(players = filter_array(inv,"filter_players",this_object()));

    armour_class = level*2;

    wander_around();

    if(!player_here) {
        if(query_hp() < query_max_hp()) {
            if(healing++ > HEALING_TIME) {
                healing = 0;
                heal_self(25);
            }     
        } else if(!stolen && !wander) {
            set_heart_beat(0);  
        } 
        if(wander && !(wander["timeout"]--))
            set_heart_beat(0);  
        return;
    }
    if(primary_attack && present(primary_attack, environment())) {
        if(helpers && !helped) {
            helped++;
            call_out("get_help", 2, primary_attack);
        }
        if(a_chat_chance > random(100)) {  
            cmd = a_chat_head[random(sizeof(a_chat_head))];
            if(cmd[0] == '*')
                command(subst(cmd[1..],"$a",(string)primary_attack->query_real_name()));
            else
                tell_room(environment(), process_msg(cmd));  
        }
        if(spell_chance > random(100) + 1) { 
            init_command(spells[random(sizeof(spells))]);
        } 
        if(query_wimpy() && query_hp() < query_max_hp()/5)
            random_move();  
    } else {
        if(chat_chance > random(100)) {  
            cmd = chat_head[random(sizeof(chat_head))];
            if(cmd[0] == '*')
                command(subst(cmd[1..],"$rn",(string)players[random(player_here)]->query_real_name()));
            else
                tell_room(environment(), process_msg(cmd));  
        }
        if(l_chat_chance > random(100) + 1) {  
            command("say "+process_msg(l_chat_head[random(sizeof(l_chat_head))]));
        }  
    }  
    if(random_pick > random(100)) {  
        pick_any_obj();  
    }  
    attack();  
}  

void get_help(object enemy) {
    int helpers_count, i, sz;
    object helper, env, *inv;
    object *storage;
    string *helpers_list;

    if(!enemy) return;
    storage = allocate(10);
    sz = sizeof(inv = all_inventory(env = environment(this_object())));
    for(i = helpers_count = 0; i < sz; i ++)
        if(living(helper = inv[i]) &&
          -1 != member_array(lower_case(helper->query_name()), helpers[1]) &&
          helper != this_object() &&
          helper != enemy && !helper->query_primary_attack())
                storage[helpers_count++] = helper;
    if(helpers_count)
        if(environment(enemy) == env) {
            if(helpers[0])
                for(i = 0; i < helpers_count; i++) {
                    if(!enemy)
                    break;
                    tell_object(enemy, sprintf(
                       "You are suddenly attacked by %s, who joins the fight.\n",
                        storage[i]->query_name()));
                    tell_room(environment(), sprintf(
                       "%s jumps into the fight and attacks %s.\n",
                        storage[i]->query_name(), enemy->query_name()), ({ enemy }));
                    storage[i]->add_secondary_attacker(enemy);
                }
        }
}
/**************************************************************************/  

status init_command(string cmd) {
    status cmd_flag;
    // If master copy don't do anything!
    if(member_array('#',file_name(this_object())) == -1) return 1;

    if(sscanf(cmd, "%s %s", skill_cmd, cmd)) {
    cmd = skill_cmd +" "+ cmd;
    }
    else {
    skill_cmd = cmd;
    }
    cmd_flag = command(cmd, this_object());
    skill_cmd = 0;
    recalc_wc();
    recalc_ac();
    return cmd_flag;
}





/***************************************************************************/  
/***   Load Chats and Attack chats.   ***/  

void load_chat(int chance, string *strs) {  
    if(!sizeof(strs)) return;     /* Just ensure that it is an array. */  
    chat_head = strs;  
    chat_chance = chance;  
}  

void load_a_chat(int chance, string *strs) {  
    if(!sizeof(strs)) return;     /* Just ensure that it is an array. */  
    a_chat_head = strs;  
    a_chat_chance = chance;  
}  

/***   Load language chat ***/  

void load_l_chat(int chance, string lang, string *strs) {  
    if(!sizeof(strs)) return;     /* Just ensure that it is an array. */  
    l_chat_head = strs;  
    l_chat_chance = chance;  
    chat_language = lang;  
    if(!chat_language){  
    if(race)  
        chat_language = lower_case(race);  
    else  
        chat_language = "common";  
    }  
}    




status death_2() {    }  



void pick_any_obj () {  
    object ob;  
    int weight;  

    ob = first_inventory(environment());  
    while (ob) {  
    if(ob->get() && ob->short()) {  
        weight = (int)ob->query_weight();  
        if(!add_weight(weight)) {  
        say (query_name()+" tries to take "+ob->short()+" but fails.\n");  
        return;  
        }  
        move_object(ob, this_object());  
        say(query_name() +" takes "+ ob->short() +".\n");  
        return;  
    }  
    ob = next_inventory(ob);  
    }  
}  

/************************************************************************/


void init() {  
    string who;
    ::init();
    if(this_player() == this_object()) return;  
    if(init_ob && init_ob->monster_init(this_object())) return;  
    if(primary_attack) set_heart_beat(1);  
    if(!this_player()->query_npc()) {  
        if(wander)  wander["timeout"] = 600;
    if(!this_player()->query_entering()) {
        who = (string)this_player()->query_name(1);
        if(!sec_att_names) sec_att_names = ({});
        if(member_array(who, sec_att_names) != -1
          || (who && member_array(capitalize(who), sec_att_names) != -1)){
        add_secondary_attacker(this_player());
        }
    }
  set_heart_beat(1);  
 if(!this_player()->query_security_level()){
  if((aggressive == 3) &&
   this_player()->query_race() != this_object()->query_race()){
           add_secondary_attacker(this_player());
  }
  if((aggressive == 2) && this_object()->query_level()*1 > 
   this_player()->query_level()){
           add_secondary_attacker(this_player());
  }
  if(aggressive == 1) { 
        string rname;
        rname = (string)this_player()->query_name(1);
        skill_cmd = "strike";
        command("strike "+ rname, this_object());
        skill_cmd = 0;
        add_secondary_attacker(this_player()); 
       } else 
       { /* not aggressive */
       if(random(100) < steal_chance) { 
        steal_from_player(this_player(),0);
       }
     }
   } // wiz loop 
  if(hate_attack && !this_player()->query_wizard()){
     if(member_array((this_player()->query_race()),hate_attack)!=-1)
        this_object()->add_secondary_attacker(this_player());
     if(member_array((this_player()->query_kingdom()),hate_attack)!=-1)
        this_object()->add_secondary_attacker(this_player());
     if(member_array((this_player()->query_alignment()),hate_attack)!=-1)
        this_object()->add_secondary_attacker(this_player());
        }
    }
}

/*** stealing from a monster, call "steal" if something is stolen... ***/ 

void steal() { 
    stolen = 1; 
    set_heart_beat(1);
} 

void steal_from_player(object who, mixed item) {
    object *inv;
    int i;
    string item_name;

    if(!item) {
    inv = all_inventory(this_player());
    if(!(i = sizeof(inv))) return;
    i = random(i);
    if(!(item_name = (string)inv[i]->query_name())) return;
    }
    else if(objectp(item)) {
    item_name = (string)item->query_name(1);
    }
    else if(stringp(item)) {
    item_name = item;
    }
    else {
    return;
    }
    skill_cmd = "steal";
    command("steal "+ item_name +" from "+ (string)who->query_name(1));
    skill_cmd = 0;
}




/*********************************************************************/
/* monster skill commands */

status skills(string str) {
    string verb;
    int i;
    verb = query_verb();
    /* verb must match current skill command */
    if(verb != skill_cmd) {
    skill_cmd = 0;
    return 0;
    }
    skill_cmd = 0;

      if(FILE_SIZE("/cmds/skills/_"+ verb +".c") > 0) {
        if(call_other("cmds/skills/_"+ verb, verb, str))
        return 1;
      }

    return 0;
}



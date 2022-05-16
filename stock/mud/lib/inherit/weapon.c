  ////////////////////////////////////////////////////////////
  // WEAPON.C         //  Crimsonesti @ // ASH.11.19.98     //
  ////////////////////////////////////////////////////////////

#include <ansi.h>
#include <mudlib.h>
inherit  BASE;
inherit "inherit/magicstuff";
#include <objlog.h>
#include <weapon.spc>


#define ALT         "external"    /* alternative object to wield this */
#define DEFAULT_LENGTH  35      /* length of weapons if not set */
#define IS_NPC  ((status)this_player()->query_npc())

status wielded;           /* 1 == weapon is wielded */  
status wielded_right;     /* 1 == weapon in right hand */    
status wielded_left;      /* 1 == weapon in left hand */
object wielded_by;        /* wielder */  
string weapon_type;       /* damaging type of the weapon */  
int    bad_ego;           /* bag ego */
string *attack_msg;       /* customized attack messages for weapon */  
string wield_str;
string room_wield_str;
int class_of_weapon;      /* weapon class of the weapon */  
int length;               /* length of weapon in inches */ 
object hit_func;          /* object containing a hit function */  
string owner;             /* special objects's may have owners */
int no_break;              /* special items may never break */

/**********************************************************************/

void reset(status arg) {
    if(arg) return;
    /* default values */

    set_name("longsword");
    set_type("slash");
    set_short("longsword");
    set_long("a rusty longsword");
    set_wc(6);
    set_weight(2);
    set_value(240);
    set_length(35);     /* 35" blade length */
}  
void set_no_break(int x){ no_break = x; }

int hit_weapon(int damage){
         if(random(175)) return 1;
         if(no_break){ 
            write("Your "+this_object()->short()+" resists breaking!\n");
            return 1;
         }

            write("Your "+this_object()->short()+" is destroyed!\n");
            log_file("BREAK", environment()->query_real_name()+
                              " "+this_object()->query_name()+
                              " "+this_object()->file_name()+" "+time()+"\n");
            this_object()->drop(1);
            environment()->recalc_wc();
            destruct(this_object());
            return 1;
    }
 


status valid_no_save()   { if(bad_ego)                             
                           return 1;                            }
status query_no_break(){ return no_break; }
status query_no_save(){ if(valid_no_save())                     return 1;
                        return ::query_no_save();               }
status ego_check()    { if(this_player()->is_npc())             return 1;
                        if(owner) return 1; 
                        if(this_player()->query_ego()           >=
                        this_object()->query_ego())             return 1;
                        return 0;                               }
void init() {  
    ::init();
    if(this_player() && environment(this_player())) 
     do_obj_log();
    call_out("id_check",1);
}

string query_owner(){ if(!owner) return ""; return owner; }
string set_owner(string o){ owner = o; return owner; }

void id_check(){
    bad_ego = 1;
    if(ego_check()) bad_ego = 0;
   if(query_owner() == "") return;
   if(this_player()->query_security() == 0 &&
      (string)this_player()->query_real_name() != query_owner()){
      move_object(this_object(),find_player(query_owner()));
      }
}  

status unwield(string wea) {  
    object left, ob, test_ob;
    string str;
    string str2;
    if(wea == "all"){ return 0; }
    if(!wea){
	ob = this_player()->query_right_weapon();
	if(ob == 0){ return 1; }
	str = ob->query_name();
	command("unwield "+lower_case(str), wielded_by);
	ob = this_player()->query_right_weapon();
	if(ob == 0){ return 1; }
	str = ob->query_name();
	command("unwield "+lower_case(str), wielded_by);
	return 1; }
    test_ob = present(wea, this_player());
    if(!test_ob){
	printf("You are not wielding a %s.\n", wea);
	return 1;
    }
    if(test_ob != this_object()) return 0;
    if(wielded) { 

	if(wielded_right) {
	    str = "You remove "+query_short()+" in your right hand";
	    str2 = this_player()->query_name()+
	    " removes "+query_short()+" in "+ this_player()->query_possessive()+
	    " right hand";
	    if(wielded_by) {
		if((left = (object)wielded_by->query_left_weapon())) {        
		    left->set_right_wielded(wielded_by);
		    str += " and wield your "+query_short()+" in your right hand";
		    str2 += " and wields "+ this_player()->query_possessive()+" "+query_short()+
		    " in "+this_player()->query_possessive()+ " right hand";
		}
	    }
	    str += ".";
	    str2 += ".";
	}
  else  {
      str = "You remove "+query_short()+" in your left hand.";
      str2 = this_player()->query_name()+
      " removes "+query_short()+" in "+ this_player()->query_possessive()+
	    " left hand.";
	}     
	ob = wielded_by;
	dewield();
	if(ob) ob->recalc_wc();  
    }  
    if(!str) str = "You aren't wielding a "+wea+".";
    printf(str + "\n");
    if(str2)
	tell_room(environment(this_player()), str2 + "\n", ({ this_player() }));
    return 1;  
}  


string query_short(){
    return query_magic_prefix()+short_desc+NORM+query_magic_suffix()+NORM; }

string short(status wiz){ return query_short(); }

status get(){
    return 1;
}



status drop(status silently) {  
    object left, ob;
    string str;
    if(wielded) { 
	if(wielded_right) {
	    str = "You drop your right weapon";
	    if(wielded_by) {
		if((left = (object)wielded_by->query_left_weapon())) {   
		    left->set_right_wielded(wielded_by);
		    str += " and wield your left weapon in your right hand.";
		}
	    }
	}
	else  {
	    str = "You drop your left weapon";
	}     
	ob = wielded_by;
	dewield();
	if(ob) ob->recalc_wc();  
	if(!silently) tell_object(ob, str +".\n");
    }  
    return 0;  
}  


/*******************************************************************/
/* query */

string query_attacker() {  
    return (string)this_player()->query_attack()->query_cap_name();  
}  

status query_wielded(string weapon_side) {
    if(weapon_side == "left") return wielded_left;
    if(weapon_side == "right") return wielded_right;
    return wielded;
}  
status query_sell_destruct () {  
    if(query_wc() > 15 || query_bonus() > 7 ||query_enchant() || 
      query_modified()) return 1;  
    return ::query_sell_destruct();  
}  

int query_true_wc() { return class_of_weapon; }
int weapon_class () { return class_of_weapon+enchant; }  
int query_wc(){
    return class_of_weapon+enchant;
}
int query_value(){
    int tmp;
    tmp = class_of_weapon + query_bonus();
    tmp = tmp*10;
    tmp = tmp * tmp;
    if(value > 10000) value = 10000; 
    if(value > tmp) return tmp;
    return value; 
}

object query_hit_func() { return hit_func; }  

string query_type()     { return weapon_type; }  
string weapon_type()    { return weapon_type; }  

int query_length()      { return length; } 

string query_object_type() { return "weapon"; }

string *query_attack_msg() { return attack_msg; }


/*******************************************************************/
/* set */


int set_wc(int wc){
    if(wc > 20) set_sell_destruct(1);
    if(wc > 30) wc = 30;
    return class_of_weapon = wc; 
}

string set_wield_str(string str){
    wield_str = str;
}
string set_room_wield_str(string str){
    room_wield_str = str;
}

void set_hit_func(object ob) {  
    hit_func = ob;  
}  

void set_right_wielded(object ob) {  /* change a left to right wielded */
    wielded = 1;
    wielded_right = 1;
    wielded_left = 0;
    wielded_by = ob;
}

string set_type(string t) { return weapon_type = t; }


int set_length(int l) { return length = l; } 

string *set_attack_msg (string *msg) {  
    sizeof(msg);
    return attack_msg = msg;  
}  



/************************************************************************/
/* misc */

/* drop(), un_wield() of right, causes left to be wielded right
* sometimes we don't want that (see wield) 
*/

void dewield() { 
    wielded = 0;
    wielded_right = 0;
    wielded_left = 0;
    wielded_by = 0;

    if(ladeda){
	ladeda = 0;
	remove_bonuses();
    }
}

void un_wield() {
    this_object()->drop(1);  
}  


int hit(object attacker) {  
    if(attacker)
    if(hit_func) return (int)hit_func->weapon_hit(attacker);  
    return 0;  
}  





/********************************************************************/
/* actions */

// wield func

status wield(string wielding) {  
    object ob, env, alt, old_weapon, opp_weapon; 
    string str; 
    int tmp_left, i, total;

    env = environment();
    if(!wielding) {
	write("wield <item>\n");
	return 1;
    }
    if(this_player()->is_player())
	if(this_player()->query_primary_attack()){
	    notify_fail(BOLD+"Not in combat!!!\n"+NORM);
	    return 1;
	}

    if(!ego_check()){
	write("You try to wield "+query_short()+" but it refuses.\n"); 
	return 1;
    }
    if(this_player()->query_left_weapon() &&
      this_player()->query_right_weapon()){
	write("You are already wielding two weapons, unwield one!\n");
	return 1; 
    }

    if(this_player()->query_armour_type("shield") &&
      this_player()->query_right_weapon() ){ 
	write("Your right and left hands are busy already.\n");
	return 1;
    }

    if(sscanf(wielding, "%s in left hand", str)) { 
	if(this_player()->query_right_weapon()) {
	    tmp_left = 1;
	}
	else {
	    write("You decide to wield it in your right hand.\n");
	}
    }
    else if(!sscanf(wielding, "%s in right hand", str)) {
	str = wielding; 
    }
    if(!(ob = present(lower_case(str), this_player()))) {
	notify_fail("You don't have a "+ str +" to wield.\n"); 
	return 0; 
    } 
    if(ob != this_object()) {
	notify_fail("You can't do that!\n");
	return 0;
    } 
    if(wielded) {
	if((wielded_left && !tmp_left) || (wielded_right && tmp_left)) {
	    drop(1);
	}
	else {
	    tell_object(env,"You already wield it!\n");  
	    return 1;  
	}
    }  
    for(i = 1; (alt = present(ALT+" "+i, this_player())); i++) {
	if(alt->wield_func(ob, tmp_left)) return 1;
    }  
    if(!IS_NPC && query_weight()* 3 > (int)env->query_stat("strength")) {  
	tell_object(env,"You lack the strength to wield that weapon.\n"); 
	return 0;  
    }  
    if(env->query_armour_type("shield") && tmp_left) {
	tell_object(env,"You cannot wield a left weapon when using a shield.\n");
	return 1;
    }
    if(tmp_left) {
	old_weapon = (object)env->query_left_weapon();
	opp_weapon = (object)env->query_right_weapon();
	total = ((opp_weapon) ? (int)opp_weapon->query_length():0)+length; 
	if(total >  ((int)env->query_stat("dexterity") *1)) {
	    tell_object(env,
	      "You try to wield these two weapons but you fumble and drop one.\n"); 
	    return 1; 
	}  
    }
    else {
	old_weapon = (object)env->query_right_weapon();
    }
    if(old_weapon) old_weapon->dewield();

    /* wield this */
    wielded_by = env;  
    wielded = 1;  
    wielded_left = tmp_left;
    wielded_right = !wielded_left;
    if(!ladeda){
	ladeda = 1;
	add_bonuses();
    }
    wielded_by->recalc_wc();
    if(tmp_left) {
	if(this_player()->is_player()) say(capitalize(this_player()->query_name()) +
	      " wields "+query_short()+".\n");
	if(!wield_str){
	    if(!sscanf(wielding, "%s/%s", str))
		write("You wield the "+query_short()+" in your left hand.\n");
	} else {
	    if(!sscanf(wielding, "%s/%s", str))
		write(wield_str);
	}
    } else {
	if(this_player()->is_player()) 
	    if(!room_wield_str)   
		if(!sscanf(wielding, "%s/%s", str))
		    say(capitalize(this_player()->query_name())+
		      " wields "+query_short()+".\n");
	if(this_player()->is_player()) 
	    if(room_wield_str)
		if(!sscanf(wielding, "%s/%s", str))
		    say(room_wield_str);
	if(!sscanf(wielding, "%s/%s", str))
	    if(!wield_str){
		write("You wield the "+query_short()+" in your right hand.\n");
	    } else {
		if(!sscanf(wielding, "%s/%s", str))
		    write(wield_str);
	    }

    }
    if(this_player()->query_left_weapon() &&
      this_player()->query_right_weapon() == 0){
	write("You have to have a weapon wielded in your right hand\n");
	write("before you can wield one in the left hand.\n");
	this_player()->query_left_weapon()->drop();
    }
    return 1;  
}  




#if 0

#define AW_ARMOUR 1
#define AW_WEAPON 2

int _w__log_item() {
    catch("/secure/aw_tracker_d"->log_item(AW_WEAPON,this_object()));
}

int _w_log_item() {
    if(!this_object() && clonep(this_object()))
	return 0;
    return call_out("_w__log_item",5),1;
}

int _w_logged = (int) _w_log_item();
int query_logged() { return _w_logged; }

#endif

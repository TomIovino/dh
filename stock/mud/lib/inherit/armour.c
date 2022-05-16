  ////////////////////////////////////////////////////////////
  // ARMOUR.C         //  Crimsonesti @ // ASH.11.19.98     //
  ////////////////////////////////////////////////////////////
 
#pragma strict_types
#pragma save_types
#pragma combine_strings
inherit "inherit/magicstuff";
#include <ansi.h>
#include <mudlib.h>
inherit  BASE;
#include <objlog.h>

#define ALT "external"
  
  
string wear_str;             /* string players see when the put on item*/
string room_wear_str;          /* room message when item is worn */
string type;                   /* armour type, also an identifier */  
status worn;                   /* flag true if worn */  
int armour_class;              /* armour class value of the armour */  
string str,str2;               /* for wear msg's */
object worn_by;                /* who wears this */  
int bad_ego;                   /* bad ego */
string owner;                  /* special objects's may have owners */
int no_break;                  /* special items may never break */


status    wear(string str);
status    remove(string str);
void      set_no_break(int x){ no_break = x; }
string    set_type(string s);
int       set_ac(int ac);
status    set_worn(status i);
object    set_worn_by(object ob);
int       armour_class();
int       query_ac();
status    query_worn();
object    query_worn_by();
string    query_type();
void      load_armour(mixed *arg);
string default_wear_msg(string arg);
   


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

int hit_armour(int damage){
         if(random(125)) return 1;
         if(no_break){
        tell_object(worn_by,                    
        "Your "+this_object()->short()+" resists breaking!\n");
             return 1;
         }
        tell_object(worn_by,                    
         "Your "+this_object()->short()+" is destroyed!\n");
            log_file("BREAK", environment()->query_real_name()+
                              " "+this_object()->query_name()+
                              " "+this_object()->file_name()+" "+time()+"\n");

         this_object()->drop(1);
         environment()->recalc_ac();
         destruct(this_object());
         return 1;
    }


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

  
void reset(status arg) {
   if(arg) return;
   set_short("suit of iron chainmail");
   set_name("chainmail");
   set_long("The chainmail looks old and rusty, "+
           "and smells like an ironworks.\n");
   set_type("armour");
   set_value(100);
   set_weight(3);
   set_ac(2);
}

status id(string str) { return ::id(str) || str == type; }
string query_object_type() { return "armour"; }

status get(){
            return 1; 
}


status drop(status silently) { 
  if(ladeda){
          ladeda = 0;
          remove_bonuses();
       }

  if(worn) {  
        if(!silently && worn_by) 
        tell_object(worn_by, "You drop your worn armour.\n");  
        worn = 0;  
if(worn_by && !worn_by->in_recalc_ac()) worn_by->recalc_ac();
    worn_by = 0;
  }
  return 0;  
}  



  /**************************************************************************/

string query_short(){ 
  return query_magic_prefix()+short_desc+NORM+query_magic_suffix()+NORM; }

string short(status wiz){ return query_short(); }

status wear(string str) {  
  object ob, env;  
  string msg1,msg2;

  env = environment();
  if(!str) {
    notify_fail("wear what?\n");
    return 0;
  }

  if(this_player()->is_player())
  if(this_player()->query_primary_attack()){
    write(BOLD+"Not in combat!!!\n"+NORM);
    return 1;
  }

  if(!ego_check()){
    write(BOLD+"You try to wear "+query_short()+" but it refuses!\n"+NORM);
       return 1;
     }
  if(this_player()->is_player())
  if(query_weight() * 3 > this_player()->query_stat("strength")){
     tell_object(env,BOLD+
     "This armour is too heavy for you to use effectively.\n"+NORM);
     return 0;
    }
  if(type == "shield"){
       if(env->query_left_weapon() && env->query_right_weapon()){
       tell_object(env,BOLD+"Your left and right hands are busy.\n"+NORM);
       return 1;
     }
  }

  if(!(ob = present(str, this_player()))) {
    notify_fail("You don't have a "+ str +".\n");
    return 0;
  }
  if(ob != this_object()) {
    notify_fail("You can't do that!\n");
    return 0; 
  }
  if(worn) {  
    tell_object(env,"You already wear it!\n");  
    return 1;  
  }  
  if(ALT) {
    object alt; int i;
    for(i = 1; (alt = present(ALT+" "+i, this_player())); i++) {
      if(alt->wear(ob)) return 1;
    }
  }
  if(env->query_armour_type(type)) {
    tell_object(env,"You already wear ");
    tell_object(env,"a "+capitalize(type)+" type armour.\n");
    return 1;
  }
   worn = 1;
   if(!ladeda){
          ladeda = 1;
         add_bonuses();
       }

  worn_by = env;
  worn_by->recalc_ac();
  if(!env->query_npc()){
  if(!default_wear_msg("me")){ 
        write("Item is broken.\n"); remove(str); return 1;}
   msg1 = default_wear_msg("me");
   msg2 = default_wear_msg("them");
   if(wear_str) msg1 = wear_str;
   if(room_wear_str) msg2 = room_wear_str;
   if(!sscanf(str, "%s/%s", str))
     write(msg1);
   if(!sscanf(str, "%s/%s", str))
     say(msg2);
   return 1;
  }
return 1;
} 
  

status remove(string str) {  
  object ob, env;  
  object alt;
  int i;

  env = environment();
  if(this_player()->query_primary_attack()){
    write(BOLD+"Not in combat!!!\n"+NORM);
    return 1;
  }

  if(!str) {
    notify_fail("remove what?\n");
    return 0;
  }
  if(str == "all"){ return 0; }
  if(!(ob = present(str, this_player()))) {
    notify_fail("You don't have a "+ str +".\n");
    return 0;
  }
  if(ob != this_object()) {
    return 0; 
  }
  if(!worn) {  
    tell_object(env,"You are not wearing it!\n");  
    return 1;  
  }  
  if(ALT) {
    for(i = 1; (alt = present(ALT+" "+i, this_player())); i++) {
      if(alt->remove_func(ob)) return 1;
    }
  }
  if(ladeda){
      ladeda = 0;
      remove_bonuses();
}
  worn = 0;
  worn_by = 0;
  env->recalc_ac();  
  tell_object(env,"You remove "+ (string)this_object()->query_short()+".\n");
  say(env->query_name() +" removes "+(string)this_object()->query_short()+".\n");
  return 1;  
}   


  /***************************************************************************/
  /***************************************************************************/
  string default_wear_msg(string arg){
  object env;
  env = environment();
  if(type == "armour"){ 
     str = ("You shrug into "+query_short()+".\n"+NORM);
     str2 =(env->query_name()+ " shrugs into "+ query_short() +".\n"+NORM); 
    }
  if(type == "shield"){ 
     str =("You hold "+query_short()+" in your left hand.\n"+NORM); 
     str2 =(env->query_name()+ " holds "+query_short()+
             " in " +env->query_possessive()+" left hand.\n"+NORM); 
    } 
  if(type == "helm"){
     str = ("You don "+query_short()+" on your head.\n"+NORM);
     str2 = (env->query_name()+ " dons "+query_short()+
             " on " +env->query_possessive()+" head.\n"+NORM);
    }
  if(type == "mask"){
     str = ("You put "+query_short()+" on your face.\n"+NORM);
     str2 = (env->query_name()+ " places "+query_short()+
             " on " +env->query_possessive()+" face.\n"+NORM);
    }
  if(type == "ring"){
     str = ("You place "+query_short()+" on your fingers.\n"+NORM);
     str2 = (env->query_name()+ " places "+query_short()+
             " on " +env->query_possessive()+" fingers.\n"+NORM);
     }
  if(type == "wrists"){
     str = ("You slide "+query_short()+" onto your wrists.\n"+NORM);
     str2 = (env->query_name()+ " slides "+query_short()+
             " onto " +env->query_possessive()+" wrists.\n"+NORM);
     }
  if(type == "ankles"){
     str = ("You slide "+query_short()+" onto your ankles.\n"+NORM);
     str2 = (env->query_name()+ " slides "+query_short()+
             " onto " +env->query_possessive()+" ankles.\n"+NORM);
     }
  if(type == "cloak"){
   str = ("You wrap "+query_short()+" about your body.\n"+NORM);
   str2 = (env->query_name()+ " wraps " +query_short()+
             " about "+env->query_possessive()+" body.\n"+NORM);
      }
  if(type == "belt"){
    str =("You wrap " +query_short()+ " about your waist.\n"+NORM);
    str2 = (env->query_name()+ " wraps "+query_short()+
          " about " +env->query_possessive()+" waist.\n"+NORM);
      }
  if(type == "boots"){
   str = ("You place "+query_short()+" on your feet.\n"+NORM); 
   str2 = (env->query_name()+ " places " +query_short()+
             " on "+env->query_possessive()+ " feet.\n"+NORM);
      }
  if(type == "gloves"){
   str =("You place "+query_short()+" on your hands.\n"+NORM);
   str2 =(env->query_name()+ " places " + query_short()+
             " on "+env->query_possessive()+" hands.\n"+NORM);
      }
  if(type == "amulet"){
   str = ("You fasten "+query_short()+" on your neck.\n"+NORM);
   str2 = (env->query_name()+ " places " + query_short()+ 
             " around "+env->query_possessive()+ " neck.\n"+NORM);
       }
  if(type == "sleeves"){
   str = ("You fasten "+query_short()+" around your arms.\n"+NORM);
   str2 = (env->query_name()+ " slips "+ query_short()+ 
             " onto "+env->query_possessive()+ " arms.\n"+NORM);

   }
   if(type == "pants"){
   str = ("You slip into "+query_short()+".\n"+NORM);
   str2 = (env->query_name()+ " slips " + query_short()+
             " onto " +env->query_possessive()+ " legs.\n"+NORM);
        }
  if(type == "floating"){
   str = ("You release "+query_short()+
    " it begins to float nearby.\n"+NORM);
     str2 = (env->query_name()+ " releases "+query_short()+
             " it begins to float.\n"+NORM);
   }

  if(arg == "me") return str;
  if(arg == "them") return str2;
}

  /****************************************************************************/  
  /* sets */

string set_type(string s)    { return type = s; }  

int set_ac(int ac) {  
   if(type == "armour" ||  "shield" ){
    if(ac > 20 || query_bonus() >  3) 
       set_sell_destruct(1);
       if(ac > 30) ac = 30;
  }  
  else if(ac > 10 || query_bonus() > 3) {  
     set_sell_destruct(1);
     if(ac > 15) ac = 15;
  }
  return armour_class = ac;           
}  
  


 

string set_wear_str(string a){
  wear_str = a;
  }
string set_room_wear_str(string a){
  room_wear_str = a;
  }

status set_worn(status i) { 
  worn = (i) ? 1 : 0;
  if(worn_by) worn_by->recalc_ac();
  if(!worn) worn_by = 0;
  return worn;
}

object set_worn_by(object ob) {
  if(ob && living(ob)) {
    worn_by = ob;
    worn = 1;
    worn_by->recalc_ac();
  }
  else {
    worn = 0;
  }
  if(worn_by) worn_by->recalc_ac();
  if(!worn) worn_by = 0;
  return worn_by;
}    

  
int armour_class()          { return armour_class+enchant; }
int query_ac ()             { return armour_class+enchant; }  
status query_worn ()        { return worn; }  
object query_worn_by()      { return worn_by; }

string query_type ()             { return type; }  
  
int query_max_value() { return value; }  
int query_value(){
        int tmp;
        tmp = armour_class + query_bonus();
        tmp = tmp * tmp * 10;
        if(value > 10000) value = 10000;
        if(value > tmp) return tmp;
        return value;
    }
  

status query_sell_destruct () {  
  if(armour_class > 13) return 1;  
  if(type != "shield" && type != "shield"  && armour_class > 5) return 1;  
  if (query_bonus() > 4) return 1;
  return ::query_sell_destruct();  
}  
  
#if 0

#define AW_ARMOUR 1
#define AW_WEAPON 2

int _a__log_item() {
    catch("/secure/aw_tracker_d"->log_item(AW_ARMOUR,this_object()));
}

int _a_log_item() {
    if(!this_object() && clonep(this_object()))
        return 0;
    return call_out("_a__log_item",5),1;
}

int _a_logged = (int) _a_log_item();
int query_logged() { return _a_logged; }

#endif

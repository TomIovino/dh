  ////////////////////////////////////////////////////////////
  // CORPSE.C         // CRIMSONESTI @  // ASH.5.17.97      //
  ////////////////////////////////////////////////////////////

#include <mudlib.h>
inherit BASE;
#include <humanoid.h>


#define DECAY_TIME 120

int playercorpse;
int decay;
string killed_by;
string race;
string corpse_name;

/* prototypes */

void decay();
void remove();

#ifdef NATIVE_MODE
void create() {
  status arg;
#else
void reset(status arg) {
#endif /* native */
  object corpse;
  string str;
  int i;
  if(arg) return;
  corpse = previous_object();
    if(!corpse) return;
  if(corpse->is_player()) playercorpse = 1; 
   if(stringp(corpse_name = (string)corpse->query_name(1))) {
        corpse_name = capitalize(corpse_name);
    } else { 
        corpse_name = "something";
    }

  set_name("corpse");
  set_short("corpse");
  set_long("This is the fresh corpse of "+ corpse_name +".\n");
  killed_by = (string)this_player()->query_name();
  race = (string)corpse->query_race();
  set_info("The wounding pattern indicates that "+str+
           " was killed by "+killed_by+".\n");
  set_smell("It smells really bad!!\n");
  set_weight(1000);
  if(sscanf(file_name(this_object()),"%s#%d",str,i)) {
    call_out("decay", DECAY_TIME);
  }
}

void decay() {
  switch(++decay) {
    case 1:
      set_short("skeleton");
      set_long("This is the skeleton of "+ corpse_name +".\n");
      set_alt_name("skeleton");
      call_out("decay", DECAY_TIME);
    break;
    case 2:
      set_short("a pile of bones");
      set_long("These bones are starched white with time.\n");
      set_name("bones");
      set_alt_name(0);
      call_out("decay", DECAY_TIME);
    break;
    case 3:
      remove();
      return;
    break;
  }
}

void remove() {
  object *inv;
  int i;

  remove_call_out("decay");
  set_weight(0);
  if(environment()) {
    inv = all_inventory();
#ifdef NATIVE_MODE
    for(i = 0; i < sizeof(inv); i++) inv[i]->move(environment());
#else
    for(i = 0; i < sizeof(inv); i++) move_object(inv[i],environment());
#endif /* NATIVE_MODE */
    environment()->recalc_carry(); /* thus fix player */
  }
  destruct(this_object());
}  

string query_killed_by() { return killed_by; }

status can_put_and_get() { return 1; }

status get()             { return 1; }

status prevent_insert() {
  write("The "+short(0)+" is too big!\n");
  return 1;
}

int query_corpse() { return 1; }
string query_race()        { return race; }
string query_corpse_name() { return corpse_name; }

void set_race(string r)        { race = r; }
void set_corpse_name(string n) { corpse_name = n; }

void init() {
  if(playercorpse) log_file("CORPSE",
      "NEAR: "+this_player()->query_name()+" near "
      +query_corpse_name()+" "+ctime()+"\n");

  add_action("component", "get");
  add_action("do_bury","bury");
}
  do_bury(str){
  if(environment() == this_player())
     { write("You have bury corpses on the ground.\n"); 
       return 1;
     }
  if(str != "corpse"){ return notify_fail("Bury what?\n"); return 0; }
  if(playercorpse) log_file("CORPSE",
      "BURY: "+this_player()->query_name()+" burried "
      +query_corpse_name()+" "+ctime()+"\n");
   write("You bury the corpse.\n");
   say(capitalize(this_player()->query_name())+" buried the corpse.\n");
   remove_call_out("decay");
   set_weight(0);
   environment()->recalc_carry();
   destruct(this_object());
   return 1;
}

string *components;

status component(string str) {
  object comp;
  if(playercorpse) log_file("CORPSE",
      "GET : "+this_player()->query_name()+" "+str+" "
      +query_corpse_name()+" "+ctime()+"\n");

  if(!str || !race || !corpse_name) return 0;
  if(sscanf(str,"%s from corpse",str) != 1) return 0;
  if(!components) components = ({});
  if(member_array(str, components) != -1) {
    if(!present(str, this_object())) {
      write("The "+ race +" has no "+ str +" that is usable.\n");
      return 1;
    }
  }
  switch(str) {
    case "tooth": case "skin": case "head": case "leg":
    case "eye":
    break;

    case "claw":
      if(humanoid(this_object())) {
        if(!present(str, this_object())) {
          write("Humanoids don't have claws!\n");
          return 1;
        }
      }
    break;

    case "arm": case "hair": case "finger nail": case "toe nail":
    case "hand": case "foot":
      if(!humanoid(this_object())) {
        if(!present(str, this_object())) {
          write("You can only get "+ str +" from humanoids.\n");
          return 1;
        }
      }
    break;

    case "scale":
      if(!(race == "fish" || race == "dragon" || race == "snake"
        || race == "reptile")) {
        if(!present(str, this_object())) {
          write("Only fish, snakes, and dragons have scales.\n");
          return 1;
        }
      }
    break;

    default:
      return 0;
    break;
  }
  comp = clone_object("inherit/food");
  comp->set_name(race +" "+ str);
  comp->set_alt_name(str);
  comp->set_alias_name(lower_case(corpse_name) +" "+ str);
  comp->set_sell_destruct(1);
  comp->set_value(1);
  comp->set_strength(0);
  comp->set_no_save(1);
  comp->set_weight(1);
  comp->set_value(5);
  comp->set_short(corpse_name +" "+str);
  comp->set_long("This is a "+ race +" "+ str +" off "+ corpse_name +".\n");
  comp->set_domain_safe(1);
  if(!this_player()->add_weight(1)) {
    write("Its too heavy for you.\n");
#ifdef NATIVE_MODE
    comp->move(environment());
#else
    move_object(comp, environment());
#endif /* NATIVE_MODE */
  }
  else {
#ifdef NATIVE_MODE
    comp->move(this_player());
#else
    move_object(comp, this_player());
#endif /* NATIVE_MODE */
  }
  components += ({ str, });
  write("You get the "+ race +"'s "+ str +".\n");
  say(this_player()->query_name() +" gets the "+ race +"'s "+ str +".\n");
  return 1;
}

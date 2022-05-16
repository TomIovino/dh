////////////////////////////////////////////////////////////
// FOOD.C             // DRAGONHEART // ASH.5.17.97       //
////////////////////////////////////////////////////////////

inherit "obj/consume";
string query_object_type() { return "food"; }
status query_food()        { return 1;      }

#ifdef NATIVE_MODE
void create() {
#else
void reset(status arg) {

  /* Default values */
  if(arg) return;
#endif /* native */

 set_weight(1);
  set_no_save(1);
  set_sell_destruct(1);
  set_name("bread");
  set_short("stale loaf of bread");
  set_long("It looks a little moldy.\n");
  set_room_msg(
    "@@query_name:$this_player()$@@ munches on some "+
    "@@query_name:$this_object()$@@.\n");
  set_consumer_msg("Yum yum!!\n");
  set_strength(1);
}

void init() {
  ::init();
  if(environment() != this_player()) return;
  add_action("eat", "eat");
}

status eat(string str) {
  object tp, ob;


  string x,y;
  tp = this_player();
  if(sscanf(this_player()->query_current_room(), "%s/arena/%s", x,y)== 2)
      {
      notify_fail("You feel way to stressed.\n");
      return 0;
      }

  if(this_player()->query_primary_attack()){
      notify_fail("Can't do that in combat!\n");
    return 0;
   }
  if(!str) { 
    write("Eat what?\n");
    return 1;
  }
  
  if(!(ob = present(str,tp))) {
    if(!str) str = "food";
    write("You don't have any "+ str +" to eat.\n");
    return 1;
  }

  if(ob != this_object()) return 0;

  if((int)tp->query_true_stat("stamina") * 4 < strength){
    write("You realize even before trying that you'll never be able\n"+ 
       "to eat all this.\n");
    return 1;
  }
  if(!tp->eat_food(strength)) return 1;
  ::consume(str);
  destruct(this_object());
  return 1;
}

void set_eater_mess(string str) { set_consumer_msg(str); }
void set_eating_mess(string str){ set_room_msg(str); }


int query_value(){ return 1; }


////////////////////////////////////////////////////////////
// BARKEEP.C          // DRAGONHEART // ASH.5.17.97       //
////////////////////////////////////////////////////////////

#include <ansi.h>
#include <mudlib.h>
inherit MONSTER;
#define OWNER       this_object()->query_name() 
#define PLAYER_STAT (int)this_player()->query_charisma()
#define OWNER_STAT  50
#define TP          this_player()
#define QN          query_name()    
object player;
mixed *drink, *food, *sdrink;
string THISR = "bartender";  // default race
int multiple = 1;  // default multiple
 
#define NAME      0
#define SHORT     1
#define LONG      2
#define MSG       3
#define MSG_ROOM  4
#define HEAL      5
 
#define AL_SIZE   6
 
 
void reset(status arg) {
  ::reset(arg);
  if(arg) return;
  set_name("bob");
  set_alias("barkeep");
  set_level(50);
  load_spells(10, ({ "heal","icelance","firelance","earthlance","airlance"}));
  set_helpers(({ "Whats this!", ({ "clerk", "guard" }), }));
  set_short("human barkeep");
  set_race("human");
  set_long("A friendly barkeeper");
#ifdef NATIVE_MODE
}
 
void create() {
#endif /* native */
set_light(2);
  drink = ({});
  food  = ({});
  sdrink= ({});
}
 
 
 
init() {
  ::init();
/*
  add_action("go",    "", 1  );
*/
  add_action("drop", "drop");
  add_action("sign",  "look" );
  add_action("sign",  "read" );
  add_action("sign",  "l");
  add_action("sell_bottle", "sell");
  add_action("sign",  "exa");
  add_action("list", "list");
  add_action("order", "buy"  );
  add_action("drop", "drop");
}
 
mixed *query_all_drinks() { return drink; }
 
 
status query_drink(string str) {
  int i;
 
  if(!drink) drink = ({});
  for(i = 0; i < sizeof(drink); i++) {
    if(str == drink[i][NAME]) return i;
  }
  return -1;
}
 
 
void add_drink(string name, string short, string drink_msg, int heal) {
  mixed *tmp;
  if(query_drink(name) != -1) return;
  tmp = allocate(AL_SIZE);
  tmp[NAME]  = name;
  tmp[SHORT] = short;
  tmp[MSG]   = drink_msg;
  tmp[HEAL]  = heal;
  drink += ({ tmp, });
}
 
 
int query_sdrink(string str) {
  int i;
 
  if(!sdrink) sdrink = ({});
  for(i = 0; i < sizeof(sdrink); i++) {
    if(str == sdrink[i][NAME]) return i;
  }
  return -1;
}
 
 
void add_sdrink(string name, string short, string drink_msg, int heal) {
  mixed *tmp;
  if(query_sdrink(name) != -1) return;
  tmp = allocate(AL_SIZE);
  tmp[NAME]  = name;
  tmp[SHORT] = short;
  tmp[MSG]   = drink_msg;
  tmp[HEAL]  = heal;
  sdrink += ({ tmp, });
}
 
 
 
status query_food(string str) {
  int i;
 
  if(!food) food = ({});
  for(i = 0; i < sizeof(food); i++) {
    if(str == food[i][NAME]) return i;
  }
  return -1;
}
 
 
void add_food(string name, string short, string drink_msg, int heal) {
  mixed *tmp;
  if(query_food(name) != -1) return;
  tmp = allocate(AL_SIZE);
  tmp[NAME]  = name;
  tmp[SHORT] = short;
  tmp[MSG]   = drink_msg;
  tmp[HEAL]  = heal;
  food += ({ tmp, });
}
 
 
 
void display_drink() {
  string tmp, money_str;
  int cost, i;
 
  for(i = 0; i < sizeof(drink); i++) {
    tmp = (!i) ? B_YELLOW_F"Alcohol:"B_WHITE_F".."NORM 
                 : B_YELLOW_F+B_WHITE_F".........."NORM;
    tmp += capitalize(drink[i][NAME]);
    tmp += ".........................................";
    tmp = extract(tmp,0,60);
    cost = drink[i][HEAL]*4 + (drink[i][HEAL]*drink[i][HEAL])/10;
    cost *= 2;
    if(!(money_str = (string)call_other(MONEY, "convert", cost))) {
      money_str = cost +"";
    }
    write(tmp +" "+ money_str +" coins\n");
    tmp = 0;
  }
  if(!i) {
    write("Alcohol: None.\n");
  }
}
 
 
void display_sdrink() {
  string tmp, money_str;
  int cost, i;
 
  for(i = 0; i < sizeof(sdrink); i++) {
    tmp = (!i) ? B_YELLOW_F"Drinks:"B_WHITE_F"..."NORM
                 : B_YELLOW_F+B_WHITE_F".........."NORM;
    tmp += capitalize(sdrink[i][NAME]);
    tmp += ".........................................";
    tmp = extract(tmp,0,60);
    cost = sdrink[i][HEAL]*4 + (sdrink[i][HEAL]*sdrink[i][HEAL])/10;
    cost *= 2;
    if(!(money_str = (string)call_other(MONEY, "convert", cost))) {
      money_str = cost +"";
    }
    write(tmp +" "+ money_str +" coins\n");
    tmp = 0;
  }
  if(!i) {
    write("Drinks: None.\n");
  }
}
 
void display_food() {
  string tmp, money_str;
  int cost, i;
 
  for(i = 0; i < sizeof(food); i++) {
   tmp = (!i) ?   B_YELLOW_F"Foods:"B_WHITE_F"...."NORM
                 : B_YELLOW_F+B_WHITE_F".........."NORM;
    tmp += capitalize(food[i][NAME]);
    tmp += ".........................................";
    tmp = extract(tmp,0,60);
    cost = food[i][HEAL]*4 + (food[i][HEAL]*food[i][HEAL])/10;
    cost *= 2;
    if(!(money_str = (string)call_other(MONEY, "convert", cost))) {
      money_str = cost +"";
    }
    write(tmp +" "+ money_str +" coins\n");
    tmp = 0;
  }
  if(!i) {
    write("Food: None.\n");
  }
}
 
 
status list(string str){
  command("read menu",this_player());
  return 1;
  }
status sign(string str) {
  if(str == "at menu" || str == "menu") {
    if(drink && sizeof(drink)) {
      display_drink();
    }
    if(sdrink && sizeof(sdrink)) {
      if(drink && sizeof(drink)) write("\n");
      display_sdrink();
    }
    if(food && sizeof(food)) {
      if((drink && sizeof(drink)) || (sdrink && sizeof(sdrink))) {
        write("\n");
      }
      display_food();
    }
    write(NORM);
    return 1;
  }
  return 0;
}
 
 
status order(string str) {
  int i;
  object beer;
  string name, short_desc, mess, type;
  string tmp1, tmp2;
  int value, cost, strength, heal;
 
 
  if(!str) {
	write(OWNER +" says: What did you want to order?\n");
    return 1;
  }
 
  if(query_drink(str) != -1) {
   i = query_drink(str);
    name       = drink[i][NAME];
    short_desc = drink[i][SHORT];
    mess       = drink[i][MSG];
    heal       = drink[i][HEAL];
    value      = heal * 3 + (heal * heal) /10;
    value += 5;
    strength   = heal;
    type       = "beer";
  }
 
   if(query_sdrink(str) != -1) {
     i = query_sdrink(str);
    name       = sdrink[i][NAME];
    short_desc = sdrink[i][SHORT];
    mess       = sdrink[i][MSG];
    heal       = sdrink[i][HEAL];
    value      = heal * 3 + (heal * heal) /10;
    value += 5;
    strength   = heal;
    type       = "soft_drink";
  }
 
  if(query_food(str) != -1) {
    i = query_food(str);
    name       = food[i][NAME];
    short_desc = food[i][SHORT];
    mess       = food[i][MSG];
    heal       = food[i][HEAL];
    value      = heal * 3 + (heal * heal) /10;
    value += 5;
    strength   = heal;
    type       = "food";
  }
 
  if(!name) {
write(OWNER +" says: What did you want to order?\n");
    return 1;
  }
 
  value = this_player()->buy(value);
  write(OWNER +" haggles with you over the price of "+ name +"...\n");
 
  if(this_player()->query_money() < value) {
    write(OWNER +" says: Hey! You haven't got "+
    (string)call_other(MONEY, "convert", value) +" coins!\n");
    return 1;
  }
 
  if(type == "beer") {
    beer = clone_object("inherit/drink");
    beer -> set_pub_drink(1);
    beer -> set_alco_drink(1);
    beer -> set_consumer_msg(mess +"\n");
    beer -> set_room_msg("@@query_name:$this_player()$@@ drinks a "+name+"\n");
  }
  else
    if(type == "soft_drink") {
      beer = clone_object("inherit/drink");
      beer -> set_consumer_msg(mess +"\n");
      beer -> set_room_msg("@@query_name:$this_player()$@@ drinks a "+name+"\n")
;
      beer -> set_pub_drink(1);
      beer -> set_soft_drink(1);
  }
  else
    if(type == "food") {
      beer = clone_object("inherit/food");
      beer -> set_consume_msg(mess +"\n");
      beer -> set_room_msg("@@query_name:$this_player()$@@ eats a "+name+"\n");
  }
 
  if(!this_player()->add_weight(beer->query_weight())) {
     write(OWNER +" says: You can't carry more!\n"+
             OWNER +" says: You'd better put on a bit more muscle.\n");
     destruct(beer);
     return 1;
  }
 
  beer -> set_value(value/2);
  beer -> set_name(name);
  beer -> set_strength(strength);
  beer -> set_short(short_desc);
  beer -> set_long(short_desc +"\n");
  if(sscanf(short_desc, "%s bowl %s", tmp1, tmp2))
    beer -> set_empty_container("bowl");
  if(sscanf(short_desc, "%s mug %s", tmp1, tmp2))
    beer -> set_empty_comtainer("mug");
  if(sscanf(short_desc, "%s plate %s", tmp1, tmp2))
    beer -> set_empty_container("plate");
 
#ifdef NATIVE_MODE
  beer->move(this_player());
#else
  move_object(beer, this_player());
#endif /* NATIVE_MODE */
  this_player()->add_money(-value);
  write(OWNER +" takes "+(string)call_other(MONEY, "convert", value)+
  " coins for a "+ name +"\n");
  say(this_player()->query_name() +" orders a "+ name +".\n");
  return 1;
}
 
go() {
  string current_verb, exit_room;
  int index;
  string *exits_here;
 
  current_verb = query_verb();
  exits_here = (string *)this_object()->query_exits();
  if(member_array(current_verb, exits_here) != -1) {
    if(has_drink(this_player())) {
      tell_object(this_player(), OWNER +" says: You're not allowed "+
        "to leave here with drinks!\n");
      return 1;
    }
  }
  return 0;
}
 
has_drink(obj) {
     int i;
     status drink;
     object ob;
 
     ob = all_inventory(obj);
     for(i=0; i<sizeof(ob); i++) {
         if(ob[i] -> id("pub drink") || ob[i] -> query_pub_drink())
             drink = 1;
        if(ob[i] -> can_put_and_get()) {
             if (has_drink(ob[i]))
                  drink = 1;
        }
     }
     return drink;
}
 
 
query_prop(string str) {
  if(str == "nulmagic") return str;
  return;
}
 
status sell_bottle(string arg) {
   object bottle;
   int coinage;
   bottle = present("bottle", this_player());
   if(arg != "bottle") {
     write(OWNER+" asks: sell what?\n");
     return 1;
   }
   if(arg == "bottle" && !bottle) {
      write(OWNER+" says: Hey! You don't have a bottle!.\n");
      return 1;
   }
   if(arg == "bottle" && bottle) {
     coinage = (int)bottle->query_value();
     coinage /= 1;
     write(OWNER+" says: Thanks for keeping the streets clean.\n");
       coinage = 4+random(3);
     write(OWNER+" hands you "+
     (string)call_other(MONEY, "convert", coinage)+
     " for the bottle\n");
     say(this_player()->query_name()+" sells the empty bottle to "+OWNER+".\n");
     this_player()->add_money(coinage);
     destruct(bottle);
     return 1;
    }
}
 
drop(str){
        object ob;
        if(!str){ return 0; }                
        if(str == "all"){
                write(OWNER+" tells you: Please do not litter in here.\n");
                return 1; }
        if(!present(str, this_player())){ return 0; }
        ob = present(str, this_player());
	if(!ob->query_object_type()) return 0;
        if(lower_case(ob->query_object_type()) == "drink"){
                write("Your drink shatters as it hits the floor.\n");
                say(TP->QN+" shatters a drink on the floor.\n", TP);
                write("You drop your drink, spilling it all over the floor.\n");
                say(TP->QN+" drops a glass, shattering it all over the floor.\n", TP);
                destruct(ob); 
                return 1; }
        if(lower_case(ob->query_object_type()) == "food"){
                write("Your food spills all over as you drop it.\n");
                say(TP->QN+" spills a tray of food all over the floor.\n",TP);
                destruct(ob);
                return 1; }
        return 0;
        }

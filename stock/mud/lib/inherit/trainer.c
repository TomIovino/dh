////////////////////////////////////////////////////////////
// TEACHER.C          // DRAGONHEART // ASH.8.11.99       //
////////////////////////////////////////////////////////////
 
#undef TEST_MODE
 
#define TP this_player()
#include <ansi.h>
#include <mudlib.h>

inherit MONSTER;
 
#define BASE_NEW_COST 250          // multiplier cost for new skills
#define BASE_INC_COST 250          // multiplier cost to increase skills
#define STUFF "/inherit/shopstuff" // get money strings
 
mixed  *classes;                   // Array of stuff trainer can teach.
string trainer;                    // Name of trainer.
int    train_level;                // Max training level of mob.
int    int_stat, int_req1, int_req2;
string str_stat, str_req1, str_req2;
string acad_name, acad_info, acad_welc;

string query_pronoun();
string query_name();
string set_stat_str(string s) { str_stat = s; }  // for skill learn
int    set_stat_int(int i)    { int_stat = i; }  // for skill learn
void   set_class_list(mixed *arg) { classes = arg; }
void   set_trainer_name(string arg) { trainer = capitalize(arg); }
void   set_train_level(int arg)   { if(arg > 25) arg = 25; train_level = arg; }
mixed  *query_class_list() { return classes; }
string query_trainer_name() { return trainer; }
int    query_train_level() { return train_level; }
string calc_value(int i);
status is_trainer() { return 1; }

int calculate_stat_value(int current_stat) {
    int value,multi;
    multi = this_player()->query_total_skills();
   if ((multi/2) > this_player()->query_level()) multi *= 4;
      else 
   if( (multi) > this_player()->query_level()) multi  *= 2;
    if(!current_stat) value =  BASE_NEW_COST*multi;
    if(current_stat)  value += BASE_INC_COST*current_stat;
    if(value < 1000)  value =  1000;   
   if(query_profession() == this_player()->query_profession())
    value = value/2;
    return value;
}
 
void LOG(string stat, int current, int cost) {
    log_file("TEACHER",
     sprintf( 
#ifdef TEST_MODE
             "<TEST> "
#endif
             "%s Trainer = %O\n\tPlayer = %O\n"
             "\tStat = %O  Curr = %O  Value = %O  Cost = %O\n",
              ctime(), this_object(), this_player(), stat, current, 
              calculate_stat_value(current), cost));
}
 
 
reset(arg) {
  NPC::reset(arg);
  if(arg) return;
  set_name(random_name());
  set_trainer_name(name);
  set_stat_str("fight");
  set_stat_int(50);
  set_alias("man");
  set_short("extremely muscular man");
  set_long("The trainer looks rather helpful, type 'ask'.\n");
  set_level(100);
  set_gender(1);
  set_train_level(10);
  set_class_list(({
    "skills", ({
   "dodge",
    }),
    "spells", ({
   "heal",
    }),
  }));
}
 
void init() {
  NPC::init();
  add_action("advance", "prac");     
  add_action("advance", "advance");
  add_action("advance", "train");
  add_action("advance","learn");
  add_action("cost", "cost");
  add_action("ask", "ask");
}
 
status cost(string str);
 
status ask(string arg, status external) {
    status just_one;
    object *trainers;
    int i;
    if(!external) {
        if(arg && arg != "") {
          if(present(arg,environment()) != this_object()) {
             return notify_fail("ask ?  or  ask who?\n");
          } else just_one++;
        } 
    }
    printf("You ask %s what %s charges for training.\n",query_name(),
               query_pronoun());
    cost("skills");
    cost("spells");
    if(just_one || external) 
      return 1;
 // else
    trainers = filter_objects(all_inventory(environment()),"is_trainer");
    trainers -= ({ this_object() });
    i = sizeof(trainers);
    if(i) while(i--) trainers[i] -> ask(0,1);
    return 1;
}

status advance(string str) {
  object tp, busy;
  int i, k, j;
  string stat;
  int fail;
  int total, current_stat;
  int value, its_cha;
  string tmp1;

  if(!str){ ask("",1);  return 1; }

  tp = this_player();

  if(busy = this_object()->query_primary_attack()) {
    return notify_fail(sprintf(
      "%s makes a motion towards you as if urging you to wait.\n", 
            query_name()));
  }

  if(tp->query_skill(str) >= 75){
    notify_fail(query_name()+
    " says: Im sorry, You cant advance to that level of skill.\n");
    return 0;
  }

  if(str_stat == "right" ||
     str_stat == "strength" ||
     str_stat == "dexterity" ||
     str_stat == "stamina" ||
     str_stat == "agility"){
    if(tp->query_skill("strength")     >= tp->query_true_stat(str_stat))
     fail = 1;
    if(tp->query_skill("stamina")      >= tp->query_true_stat(str_stat))
     fail = 1;
    if(tp->query_skill("dexterity")    >= tp->query_true_stat(str_stat))
     fail = 1;
    if(tp->query_skill("agility")      >= tp->query_true_stat(str_stat))
     fail = 1;
    if(fail){
    notify_fail(query_name()+
    " says: Im sorry, Your incapable of earning that level of skill.\n");
    return 0;
      } 
    }




  if(str_stat == "left" ||
     str_stat == "intellect" ||
     str_stat == "sense" ||
     str_stat == "wisdom" ||
     str_stat == "willpower"){
    if(tp->query_skill("intellect")    >= tp->query_true_stat(str_stat))
     fail = 1;
    if(tp->query_skill("sense")     >= tp->query_true_stat(str_stat))
     fail = 1;
    if(tp->query_skill("wisdom")    >= tp->query_true_stat(str_stat))
     fail = 1;
    if(tp->query_skill("willpower")  >= tp->query_true_stat(str_stat))
     fail = 1;
    if(fail){
    notify_fail(query_name()+
    " says: Im sorry, Your incapable of earning that level of skill.\n");
    return 0;
      } 
    }
 /**
  if(tp->query_true_stat(str_stat) < int_stat){
    notify_fail(query_name()+
    " says: You must increase your "+str_stat+" before I can help you.\n");
    return 0;
  }
**/

  if(tp->query_npc()) {
    tell_room(environment(tp), sprintf(
        "%s laughs heartily!\n", 
          query_name()));
     return 1;
  }
  for( i = 0; i < sizeof(classes); i += 2 ) {
    if(( j = member_array(str, classes[i+1])) != -1) {
    stat = str;

    stat = lower_case(stat);
    current_stat = (int)this_player()->query_skill(stat);
/*
    if(current_stat >= train_level) {
      return notify_fail(sprintf(
       "%s%s says:%s You will have to train elsewhere since your skill in\n"
       "\t%s is higher than I can teach it.\n", CYAN_F,query_name(),NORM,str));
    }
*/
    value = calculate_stat_value(current_stat);
    value = this_player()->buy(value);
    printf("%s haggles you over the price of training.\n", query_name());
 
    if(tp->query_money() < value) {
      return notify_fail(sprintf(
         "%s frowns at you.\n"
         "%s%s says:%s You seem to be a little short on funds.\n",
            query_name(),CYAN_F,query_name(),NORM));
    }
    printf("%s takes %s from you for training.\n",
      query_name(), calc_value(value));
    stat = str;
LOG(stat, current_stat, value);
#ifdef TEST_MODE
    printf("TEST_MODE  tp->set_%s(%O); would have been called\n",stat,current_stat+1);
    printf("TEST_MODE  tp->add_money(%O); would have been called\n",-value);
    printf("TEST_MODE  tp->save_me(0); would have been called\n");
#else
    this_player()->adj_skill(stat,1);
    tp->add_money(-value);
    tp->save_me(0);
#endif
    printf("%s helps you raise your %s.\n", query_name(), str);
    tell_room(environment(tp), sprintf(
       "%s helps %s raise %s %s.\n", query_name(), tp->query_name(),
      tp->query_possessive(), str), ({ tp }));
    return 1;
  
  }
  }
  return notify_fail(
    sprintf("%s%s says:%s You will have to go elsewhere to learn that.\n",
    CYAN_F,query_name(),NORM));
}
 
status cost(string str) {
  object busy, tp;
  string *its_lang;
  int i, k, j;
  string stat;
  int total, current_stat;
  int value, its_cha;
  string tmp1;
 
  tp = this_player();
  if(busy = this_object()->query_primary_attack()) {
    return notify_fail(sprintf(
  "%s makes a motion towards you as if urging you to wait.\n", query_name()));
  }
  if(!str) {
    return notify_fail(sprintf(
    "%s%s says:%s I can give you costs on skills or spells.\n",
    CYAN_F, query_name(), NORM));
  }
  if((i = member_array(str, classes)) != -1) {
      for(k = 0; k < sizeof(classes[i+1]); k++ ) {
          cost(classes[i+1][k]);
      }
      return 1;
  } else {
    for( i = 0; i < sizeof(classes); i += 2 ) {
        if(( j = member_array(str, classes[i+1])) != -1) {
            stat = str;
            stat = lower_case(stat);
    current_stat = (int)this_player()->query_skill(stat);
            if(current_stat >= train_level) {
               printf(
       "%s%s says:%s You will have to train elsewhere since your skill in\n"
            "\t%s is higher than I can teach it.\n", CYAN_F,
                    query_name(),NORM,str);
                 return 1;
            }
            value = calculate_stat_value(current_stat);
            printf("%s%s says:%s For %s I will charge you %s.\n",
         CYAN_F, query_name(), NORM, str, calc_value(value*2));
            if(tp->query_money() < value) {
              printf("\t%sYou will need %s more.%s\n",
                  B_RED_F,calc_value((value - tp->query_money())), NORM);
            }
            return 1;
      }
    }
  }
  return notify_fail(
    sprintf("%s%s says:%s You will have to go elsewhere to learn that.\n",
    CYAN_F,query_name(),NORM));
}
 
 
 
string calc_value(int i) {
  int g, s, c;
  g = s = c = 0;
  if(i >= 100) { g = i / 100; i %= 100; }
  if(i >= 10)  { s = i /  10; i %=  10; }
  if(i)         { c = i;                   }
  return STUFF->money_string(g,s,c);
}

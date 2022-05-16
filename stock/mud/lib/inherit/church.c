////////////////////////////////////////////////////////////
// CHURCH.C           // DRAGONHEART // ASH.5.17.97       //
////////////////////////////////////////////////////////////


#include <ansi.h>
#include <mudlib.h>
#include <bar.h>

#define PASS BOLD+BLUE_F
#define FAIL RED_F

#define DEX TP->query_true_stat("dexterity")
#define AGI TP->query_true_stat("agility")
#define STR TP->query_true_stat("strength")
#define STA TP->query_true_stat("stamina")

#define WIS TP->query_true_stat("wisdom")
#define FOC TP->query_true_stat("focus")
#define INT TP->query_true_stat("intellect")
#define PRI TP->query_true_stat("primal")

inherit "/inherit/tell_all";
inherit ROOM;


#define TP this_player() 
query_modified(){ return 1; } // modified room types

void reset(status arg) {
  ::reset(arg);
  if(arg) return;
  add_object("mana rune","/cmds/skills/mana");
  add_object("health rune","/cmds/skills/health");
  add_object("movement rune","/cmds/skills/movement");

  set_short("The Temple");
  set_long("\
A huge octagonal room that is constructed of white marble. The upper\n\
gallery consists entirely of windows that are filled with stained glass\n\
windows which allows the sun to turn the marble into jewel-like colors.\n\
Echoes repeat themselves over and over creating a soft murmur that is\n\
constantly renewing itself, almost as if the room was speaking. Slender\n\
columns rise from the ground at each corner of the octagon, framing eight\n\
crystalline statues of dragons at each of the corners of the room.\n\
  ");
   set_light(5);
   set_no_magic(1);
   set_no_fight(1);
   set_light(3);
   set_exits(({
  "/d/forest/m/15","down",
  }));
}




void init(){

 string check;

 ::init();

 if(!this_player()->query_security_level())
/*
 if(query_type() != this_player()->query_alignment())
     command("south",this_player());
*/

 if(TP->is_player())
 if(TP->shouldpray() > 0){
  write(
  NORM+"You feel as though you can pray "+TP->shouldpray()+" times.\n"+NORM);
  } 

  add_action("pray_fun","pray"); 
  add_action("so","east");
  add_action("so","west");
  add_action("so","north");
  add_action("so","south");
  add_action("unlearn","unlearn");
}


status unlearn(string what){
  if(!what){ 
    write("Unlearn which skill?\n"); 
  return 1; }
    write("You cant unlearn that!\n");
  return 1;}

status pray_for_skills(string str){
  object pl;
   pl = find_player(str);
   }
       
status so(string str){
     if(this_player()->query_earned_percent() > 100 &&
      this_player()->query_level() < 100){
      printf(FAIL+"\
You feel you should 'pray for levels' before leaving the temple.\n"+NORM);
      return 1;
     }
     if(this_player()->query_dead()) {
      printf(FAIL+"\
You feel the need to 'pray for life' before leaving the temple.\n"+NORM);
      return 1;
     }
           
}

status pray_fun(string str){
 string a,b;
 if(TP->query_position() != "kneeling"){
    write("HA, on your knees mortal, when you wish an audience with God!\n");
    return 1;
 }

 if(TP->query_alignment() == "earth"){  a = "ranger";    b = "druid"; }
 if(TP->query_alignment() == "water"){  a = "hunter";    b = "mystic"; }
 if(TP->query_alignment() == "air"){    a = "sentry";    b = "sorcerer"; }
 if(TP->query_alignment() == "fire"){   a = "samurai";   b = "monk"; }
 if(TP->query_alignment() == "death"){  a = "vampire";   b = "lich"; }
 if(TP->query_alignment() == "life"){   a = "paladin";   b = "highpriest"; }
 if(TP->query_alignment() == "chaos"){  a = "berserker"; b = "seeker"; }
 if(TP->query_alignment() == "order"){  a = "enforcer";  b = "arcane"; }


#define STATS   ({ "wisdom","intellect","primal","focus", \
                   "dexterity","strength","stamina","agility" })

 if(TP->query_age())
   if(!TP->query_attrib("luckymark")){
   int rx;
   string rs;
   rx = random(sizeof(STATS));
   rs = STATS[rx];
   write("\n"+PRINTBAR+"\n");
   write(TP->query_who()+",\n"+TP->query_color()+
   "The power of "+rs+" favors your first birthday!\n"+
   "In honor of this the spirits grant you the mark of "+rs+".\n");
   TP->set_attrib("luckymark",rs);
   write("\n"+PRINTBAR+"\n");
   return 1;
 }
 if(TP->query_level() > 49)
  if(TP->query_title() == "lad" || TP->query_title() == "lass"){
   write("\n"+PRINTBAR+"\n");
   write(TP->query_who()+",\n"+TP->query_color()+
   "It is time for you to take your place among the nobility.\n");
   write("You have been promoted to the rank of:\n");
   write(TP->query_who()+".\n");
   TP->update_pre_title();
   write("\n"+PRINTBAR+"\n");
   return 1;
 }

 if(str == "lev" || str == "level" 
   || str == "for levels" || str == "for level"){
    printf("You pray for increased power.\n");
    this_player()->add_exp(0);
    return 1;
 }

 if(str == "life" || str == "for life"){
      if(this_player()->query_dead()) {
        this_player()->toggle_death();
          write("You pray for mercy and a second chance\n");
        return 1;
      } else {
        return notify_fail(FAIL+"You waste the gods' time, be gone, "
                           "for you are alive.\n"+NORM),0;
      }
 }


/*------------------------SET_STATS------------------------------------*/
if(TP->shouldpray() < 1){  
        return notify_fail(FAIL+"For such a prayer to be heard mortal,\n"
        "you must earn it. Come back when you have advanced a level.\n"),0;
}

/*-------------------------STRENGTH-------------------------*/ 
  if(str == "str" || str == "strength" || str == "for strength") {
    printf("You prayed for Strength.\n");   
  if(TP->query_stat("strength") >= TP->query_max_stat("strength") ){
    printf("\
You step up to the altar and pray for strength but realize that your \n\
prayers are never heard.\n");
    return 1;
  }
  if(TP->query_last() == "strength"){
    printf("The gods think you should pray for something else.\n");
   return 1;
}
        printf("You feel stronger.\n");
        TP->adj_stat("strength",1);
        return 1;
  }
/*-------------------------WISDOM--------------------------*/ 
 if(str=="wis"||str=="wisdom"||str=="for wisdom") {
        printf("You prayed for Wisdom.\n");
  if( (TP->query_stat("wisdom")) >= TP->query_max_stat("wisdom")){
    notify_fail("You cannot become any wiser.\n");
    return 0;
  }
  if(TP->query_last() == "wisdom"){
    printf("The gods think you should pray for something else.\n");
   return 1;
}

    printf("You feel wiser.\n");
        TP->adj_stat("wisdom",1);
        return 1;
  }
/*------------------------INTELLECT--------------------------*/ 

  if(str=="int"||str=="intellect"||str=="for intellect") {
        printf("You prayed for Intellect.\n");
  if( (TP->query_stat("intellect")) >= TP->query_max_stat("intellect")){
    notify_fail("You cannot become any more intelligent.\n");
    return 0;
  }
  if(TP->query_last() == "intellect"){
    printf("The gods think you should pray for something else.\n");
   return 1;
}
        printf("You feel more intellect\n");
        TP->adj_stat("intellect",1);
        return 1;
  }
/*------------------------PRIMAL-------------------------*/ 

  if(str=="pri"||str=="primal"||str=="for primal") {
        printf("You prayed for primal power.\n");
  if( TP->query_stat("primal") >= TP->query_max_stat("primal")){
    notify_fail("You cannot become more primal.\n");
    return 0;
  }
  if(TP->query_last() == "primal"){
    printf("The gods think you should pray for something else.\n");
   return 1;
}

        printf("You feel more primal.\n");
        TP->adj_stat("primal",1);
        return 1;
  }
/*-------------------------STAMINA--------------------------*/ 

  if(str=="sta"||str=="stamina"||str=="for stamina") {
        printf("You prayed for Stamina.\n");
  if( (TP->query_stat("stamina")) >= TP->query_max_stat("stamina")){
    notify_fail("You cannot have a greater stamina.\n");
    return 0;
  }
  if(TP->query_last() == "stamina"){
    printf("The gods think you should pray for something else.\n");
   return 1;
}

        printf("You feel healthier.\n");
        this_player()->adj_stat("stamina",1);
        return 1;
  }


/*-------------------------FOCUS--------------------------*/ 

  if(str=="foc"||str=="focus"||str=="for focus") {
        printf("You prayed for focus.\n");
  if( (TP->query_stat("focus")) >= TP->query_max_stat("focus")){
    notify_fail("You cannot have a greater focus.\n");
    return 0;
  }
  if(TP->query_last() == "focus"){
    printf("The gods think you should pray for something else.\n");
   return 1;
}

        printf("You feel more focused.\n");
        this_player()->adj_stat("focus",1);
        return 1;
  }
/*-------------------------AGILITY--------------------------*/ 

  if(str=="agi"||str=="agility"||str=="for agility") {
        printf("You prayed for agility.\n");
  if( (TP->query_stat("agility")) >= TP->query_max_stat("agility")){
    notify_fail("You cannot have a greater agility.\n");
    return 0;
  }
  if(TP->query_last() == "agility"){
    printf("The gods think you should pray for something else.\n");
   return 1;
}

        printf("You have higher agility.\n");
        this_player()->adj_stat("agility",1);
        return 1;
  }

/*-------------------------DEXTERITY-------------------------*/ 

  if(str=="dex"||str=="dexterity"||str=="for dexterity") {
       printf("You prayed for increased dexterity.\n");
  if( (TP->query_stat("dexterity")) >= TP->query_max_stat("dexterity")){
    notify_fail("You cannot raise your dexterity any more.\n");
    return 0;
  }
  if(TP->query_last() == "dexterity"){
    printf("The gods think you should pray for something else.\n");
   return 1;
}
       printf("You feel more dexterous\n");
       this_player()->adj_stat("dexterity",1); 
       return 1;
       }
  
    write("You mutter a little prayer to the Gods.\n");
    return 1;
}



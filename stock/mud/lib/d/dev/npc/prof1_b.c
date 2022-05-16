
#include <mudlib.h>
inherit TEACHER;
#include <ansi.h>
#include "../element.h"

int u,e;

reset(arg){
    ::reset(arg);
    if(arg) return;
    set_alias(RACE);
    set_name("master");
    set_alignment(ELEMENT);
    set_race(RACE);
    set_profession(PROF1);
    set_short(COLOR+RACE+" "+PROF1+" master"+NORM);
    set_long("A "+PROF1+" of the realm of "+ELEMENT+".\n"+
    "This being is very wise in the ways of is trade being a\n"+
    "master among its kind. You would do well to train here.\n");
    set_gender(1+random(2));
    set_level(90);
  if(query_gender() == 1){ 
    load_chat(1,({ "*bow","*smile", }));
    }
  if(query_gender() == 2){
    load_chat(1,({ "*curtsey","*greet", }));
    }
    add_money(50);
    set_wimpy(50);
    set_magic_resist(33);
    set_helpers(({ "", ({ "guard", }) , }));
    add_object("OB1",PATH+"obj/3");
    add_object("OB1",PATH+"obj/3weapon");
    load_spells(10,({
       "heal",
      }));
    set_train_level(50);
    set_stat_int(25);
    set_stat_str("constitution");
    set_class_list(({
        "skills", ({
 "strike","kick","punch","circle","stealth tactics",
 "defensive tactics", "block",
 "multi opponents","multi attack",
 "enhanced damage","offensive tactics",
 "shield block", "shield crush",
        }),
      }));


}

init(){
 ::init();
 if(!this_player()->query_alt_id(ELEMENT) &&
       !this_player()->query_security_level()){
        this_object()->add_secondary_attacker(this_player()); 
 }
}

enter(){
}

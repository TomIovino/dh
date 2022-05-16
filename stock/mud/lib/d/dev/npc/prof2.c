
#include <mudlib.h>
inherit TEACHER;
#include <ansi.h>
#include "../element.h"

int u,e;

reset(arg){
    ::reset(arg);
    if(arg) return;
    set_alias(RACE);
    set_name(PROF2);
    set_alignment(ELEMENT);
    set_race(RACE);
    set_profession(PROF2);
    set_short(COLOR+RACE+" "+PROF2+" instructor"+NORM);
    set_long("A "+PROF2+" of the realm of "+ELEMENT+".\n"+
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
    add_object("OB3",PATH+"obj/3");
    load_spells(10,({
       "heal",
      }));
    set_train_level(50);
    set_stat_int(25);
    set_stat_str("willpower");
    set_class_list(({
        "skills", ({
    "casting tactics","research","quick cast","quiet cast","meditate",
          }),
         "spells", ({
    "grasp","flash","lance","bolt","jolt","spike","burst","blast","storm",
    "return","visit","scry","turn","message","summon","gate","teleport",
    "health rune","psychic rune","barrier seal","magic seal","privacy seal",
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

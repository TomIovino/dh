
#include <mudlib.h>
inherit MONSTER;
#include <ansi.h>
#include "../element.h"

int u,e;

reset(arg){
    ::reset(arg);
    if(arg) return;
    set_name("guard");
    set_alias(RACE);
    set_alignment(ELEMENT);
    set_short(COLOR+RACE+" guard");
    set_long("A guard of the realm of "+ELEMENT+", this "+
    RACE+"\nnotices your glance and continues about its duty.\n"); 
    set_gender(1+random(2));
    set_level(75);
  if(query_gender() == 1){ 
    load_chat(1,({ "*bow","*smile", }));
    }
  if(query_gender() == 2){
    load_chat(1,({ "*curtsey","*greet", }));
    }
    add_money(50);
  if(random(1))
    set_race(RACE);
    set_wimpy(50);
    set_magic_resist(33);
    set_helpers(({ "", ({ "guard", }) , }));
    add_object("OB1",PATH+"obj/2");
    add_object("OB2",PATH+"obj/2");
}

init(){
 ::init();
 if(!this_player()->query_alt_id(ELEMENT) &&
       !this_player()->query_security_level()){
        this_object()->add_secondary_attacker(this_player()); 
 }
 add_action("enter","east");
 add_action("enter","west");
 add_action("enter","south");
 add_action("enter","north");
}

enter(){
    if(!this_player()->query_alt_id(ELEMENT) &&
       !this_player()->query_security_level()){
        write(COLOR+capitalize(RACE)+" guard blocks your path!\n");
        return 1;
      }
}

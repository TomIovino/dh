
#include <mudlib.h>
inherit MONSTER;
#include <ansi.h>
#include "../element.h"

int u,e;

reset(arg){
    ::reset(arg);
    if(arg) return;
    set_gender(1+random(2));
    set_level(10);
  if(query_gender() == 1){ 
    load_chat(1,({ "*bow","*smile", }));
    }
  if(query_gender() == 2){
    load_chat(1,({ "*curtsey","*greet", }));
    }
    add_money(50);
    set_name("citizen");
    set_alias(RACE);
    set_alignment(ELEMENT);
  if(random(1))
    set_short(COLOR+RACE+" citizen");
    set_short(COLOR+RACE+" citizen");
    set_long("A simple denizen of the realm of "+ELEMENT+", this "+
    RACE+"\nseems too busy to notice your glace.\n"); 
    set_race(RACE);
    set_wimpy(50);
    add_object("OB1",PATH+"obj/1");
    add_object("OB2",PATH+"obj/1");
}

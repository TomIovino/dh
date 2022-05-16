
#include <mudlib.h>
inherit MONSTER;
#include <ansi.h>
int u,e;

reset(arg){
    ::reset(arg);
    if(arg) return;
    add_object("NEWBIE_OB","d/town/obj/NEWBIE_OB");
    add_object("NEWBIE_OB","d/town/obj/NEWBIE_OB");
    set_gender(1+random(2));
    set_wander(50, 30, 0, "/d/town");
    set_level(25);
  if(query_gender() == 1){ 
    load_chat(1,({ "*bow","*smile", }));
    }
  if(query_gender() == 2){
    load_chat(1,({ "*curtsey","*greet", }));
    }
    add_money(50);
    set_name(random_name());
    set_alias("citizen");
    set_half_exp(1);
    set_short("twilite dragonkin citizen");
    set_long(
      "A fairly ordinary citizen of Twilight going about "+this_object()->query_possessive()+" daily\n"+
      "business. "+capitalize(this_object()->query_pronoun())+" is dressed simply but neatly, and would appear\n"+
      "to have had a quiet, comfortable life.\n");
    set_race("dragonkin");
    set_wimpy(50);
}

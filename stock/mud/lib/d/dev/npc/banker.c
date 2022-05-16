// Ash

#include <mudlib.h>
#include <ansi.h>
#include "../element.h"
inherit BANKKEEP;


reset (arg){
   if(arg) return;
   ::reset(arg);
   add_object("OB3",PATH+"obj/3");
   set_name("banker");
   set_alias(RACE);
   set_alignment(ELEMENT);
   set_short(COLOR+RACE+" banker"+NORM);
   set_long(COLOR+
   "Here stands the the proud owner of "+BANK+".\n"); 
   set_race(RACE);
   set_gender(random(1)+1);
   set_level(90);
   load_spells(15, ({ "heal", }));
   add_money(((random(100)+400)*2)+1);

}

init(){
 ::init();
 if(!this_player()->query_alt_id(ELEMENT) &&
       !this_player()->query_security_level()){
        this_object()->add_secondary_attacker(this_player());
 }
}


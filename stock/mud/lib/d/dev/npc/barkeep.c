// Ash

#include <mudlib.h>
#include <ansi.h>
#include "../element.h"
inherit BARKEEP;


reset (arg){
   if(arg) return;
   ::reset(arg);
   add_object("OB3",PATH+"obj/3");
   set_name("barkeep");
   set_alias(RACE);
   set_alignment(ELEMENT);
   set_short(COLOR+RACE+" barkeep"+NORM);
   set_long(COLOR+
   "Here stands the the proud owner of "+PUB+".\n"); 
   set_race(RACE);
   set_gender(random(1)+1);
   set_level(90);
   load_chat(1, 
   ({ COLOR+"Barkeep asks you for your order.\n"NORM }));
   load_spells(15, ({ "heal", }));
   add_money(((random(100)+400)*2)+1);
   add_drink("ale", "glass of ale", "AH.. imported.",25);
   add_food("jerky", "beef jerky", "Its chewy.",25);
   add_sdrink("water", "cup of water", "AH.. that was good.",25);

}

init(){
 ::init();
 if(!this_player()->query_alt_id(ELEMENT) &&
       !this_player()->query_security_level()){
        this_object()->add_secondary_attacker(this_player());
 }
}


// Ash

#include <mudlib.h>
#include <ansi.h>
#include "../element.h"
inherit SHOPKEEP;


reset (arg){
   if(arg) return;
   ::reset(arg);
    STOREROOM = PATH+"room/warehouse";
    SIGN_TEXT = capitalize(query_name())+" says, what do you want?";
   add_object("OB3",PATH+"obj/3");
   set_name("shopkeep");
   set_alias(RACE);
   set_alignment(ELEMENT);
   set_short(COLOR+RACE+" shopkeep"+NORM);
   set_long(COLOR+
   "Here stands the the proud owner of "+SHOP+".\n"); 
   set_race(RACE);
   set_gender(random(1)+1);
   set_level(90);
   load_chat(1, 
   ({ COLOR+"Shopkeep points to the 'list' of goods on the wall.\n"NORM }));
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


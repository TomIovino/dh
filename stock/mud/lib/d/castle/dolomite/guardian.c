#include <mudlib.h>
inherit BANKKEEP;
#include <ansi.h>
#include "hdefs.h"
int u,e;


reset(arg){
    ::reset(arg);
    if(arg) return;
    set_name("guardian");
    set_alias(HOUSE);
    set_alignment(HOUSE);
    set_short(""CASTLE" guardian");
    set_gender(GENDER);
    add_object("thrust_2","d/town/obj/thrust_2");
    add_object("slash_2","d/town/obj/slash_2");
    add_object("placetaker","/d/town/obj/CLOTHING");        
    set_long("A guardian of the House of "CASTLE", this "
            RACE" "+query_gender_string()+" guardian\n"+
          "is lightly armoured but is fully trained to guard the House's\n"+
     "treasures.\n"+NORM);
    set_level(100);
    if(query_gender() == 1){ 
    load_chat(1,({ "*bow","*smile", }));
    }
    load_spells(10, ({ "cure","lance" }));
    if(query_gender() == 2){
    load_chat(1,({ "*curtsey","*greet", }));
    }
    set_race(RACE);
    set_half_exp(1);
    set_helpers(({ "", ({ "guard", }) , }));
}


init(){
 ::init();
     if(!this_player()->query_security_level())
     if(!HOUSED->query_member(HOUSE,this_player()->query_real_name()) && 
     !HOUSED->query_ally(HOUSE,this_player()->query_real_name())){
         if((string)file_name(environment(this_object())) ==
            (string)this_object()->query_create_room()){
             if(this_player()->is_player())
             add_secondary_attacker(this_player());
     add_action("enter","north");
     add_action("enter","east");
     add_action("enter","west");
         }
     }

}



enter(){
    if(!this_player()->query_security_level())
     if(!HOUSED->query_member(HOUSE,this_player()->query_real_name()) && 
        !HOUSED->query_ally(HOUSE,this_player()->query_real_name())){
         if((string)file_name(environment(this_object())) ==
            (string)this_object()->query_create_room()){
            write(""CASTLE" guardian blocks your path!\n");
            return 1;
            }
         return 0;
     }
    return 0;
}


#include <mudlib.h>
inherit MONSTER;
#include <ansi.h>
#include "hdefs.h"
int u,e;


reset(arg){
    ::reset(arg);
    if(arg) return;
    set_name("knight");
    set_alias(HOUSE);
    set_alignment(HOUSE);
    set_short(""CASTLE" knight");
    add_object("cleave_2","d/town/obj/cleave_2");
    add_object("shield_2","d/town/obj/shield_2");
    add_object("placetaker","/d/town/obj/CLOTHING");        
    set_gender(GENDER);
    set_long("A knight of the House of "CASTLE", this "
            RACE" "+query_gender_string()+" knight\n"+
             "is lightly armoured but is a fully trained knight.\n"+NORM);
    set_level(75);
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
            write(""CASTLE" knight blocks your path!\n");
            return 1;
            }
         return 0;
     }
    return 0;
}


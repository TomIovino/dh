#include <mudlib.h>
inherit MONSTER;
#include <ansi.h>
#include "hdefs.h"
int u,e;


reset(arg){
    ::reset(arg);
    if(arg) return;
    set_name("sentry");
    set_alias(HOUSE);
    set_alignment(HOUSE);
    add_object("crush_2","d/town/obj/crush_2");
    add_object("shield_2","d/town/obj/shield_2");
    add_object("placetaker","/d/town/obj/CLOTHING");        
    set_short(""CASTLE" sentry");
    set_gender(GENDER);
    set_long("A sentry of the House of "CASTLE", this "
            RACE" "+query_gender_string()+" sentry\n"+
             "is lightly armoured but is a fully trained mercenary.\n"+NORM);
    set_level(60);
    load_chat(5,({
    "Sentry says: Halt, and be recognized!\n",
    "Sentry says: Strangers are not welcome at Castle "CASTLE".\n",
    }));
    load_spells(10, ({ "cure","lance" }));
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
            catch(CHAND->
            tell_channel(HOUSE,this_player()->query_alt_name(),
                    ":has entered our lands!\n"));
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
            write(""CASTLE" sentry blocks your path!\n");
            return 1;
            }
         return 0;
     }
    return 0;
}


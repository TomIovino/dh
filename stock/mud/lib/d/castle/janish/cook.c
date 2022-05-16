#include "hdefs.h"
#include <mudlib.h>
#include <ansi.h>

inherit BARKEEP;
reset(arg) {
    ::reset(arg);
    if(arg) return;
    set_name("cook");
    add_object("placetaker","/d/town/obj/CLOTHING");        
    set_short(WHITE_F+CASTLE+" cook"+NORM);
    set_alias("barkeep");
    set_long(
    "The "CASTLE" cook bustles around his kitchen serving food and drink to\n"+
    "his customers. His ample stomach shows that he enjoys his own cooking.\n"+
    "Type 'list' to see the items available for purchase.\n");
    set_race(RACE);
    set_gender(1);
    load_chat(1,({   
        "*smile","*greet",
    }));
    add_drink("ale","bottle of ale", "The ale goes down smooth.", 2);
    add_drink("wine", "glass of fine wine", "You get a warm feeling.",5); 
    add_drink("spirits","bottle of dragon spirits","A shockwave shocks you like a wave!",20);
    add_food("soup", "bowl of vegetable soup", "UMM UMMM good.",2);
    add_food("cheese", "lumpy piece of cheese", "It tastes good.",5);
    add_food("bread", "loaf of bread", "It tastes a little overdone.",20);
    add_sdrink("water", "cup of water", "AH.. that was good.",2);
    add_sdrink("juice", "glass of fruit juice", "Tasty and refreshing.", 5);
    add_sdrink("nectar", "small vial of nectar","That was different!",20);
    set_level(90);
    set_magic_resist(50);

}

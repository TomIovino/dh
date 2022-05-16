#include <mudlib.h>
inherit BARKEEP;
#include <ansi.h>

reset(arg) {
    ::reset(arg);
    if(arg) return;
    add_object("NOBLE_OB","d/town/obj/NOBLE_OB");
    set_name("grog");
    set_short(CYAN_F+"dragonkin barkeep"+NORM);
    set_alias("barkeep");
    set_long(
    "This is one ugly looking Dragonkin, and also the owner and bartender of\n"+
    "the Old Shirax Pub, now established here in Twilite City. Grog is a\n"+
      "big ugly dragonkin with one blood-shot yellow eye, and a huge pot belly\n"+
      "barely concealed by a dirty, greasy apron. His bare arms and shoulders\n"+
      "are covered with thick hair, and he drools quite a bit.\n");
    set_race("dragonkin");
    set_gender(1);
    load_chat(1,({   
    "Grog says : I wouldn't step foot in Shirax anymore if I were you.\n",
    "Grog says : I see you're from the fringelands, beyond our civil lands.\n",
    "Grog says : This is my home since the King granted me Citizenship.\n",
        "*smile","*greet",
    }));
    add_drink("beer","bottle of beer", "The beer goes down smooth.", 2);
    add_drink("wine", "glass of fine wine", "You get a warm feeling.",5); 
    add_drink("spirits","bottle of dwarven spirits","A shockwave shocks you like a wave!",20);
    add_food("soup", "bowl of vegetable soup", "UMM UMMM good.",2);
    add_food("cheese", "lumpy piece of cheese", "It tastes good.",5);
    add_food("bread", "loaf of bread", "It tastes a little overdone.",20);
    add_sdrink("water", "cup of water", "AH.. that was good.",2);
    add_sdrink("juice", "glass of fruit juice", "Tasty and refreshing.", 5);
    add_sdrink("nectar", "small vial of nectar","That was different!",20);
    set_level(50);
    set_half_exp(1);
    set_magic_resist(50);

}

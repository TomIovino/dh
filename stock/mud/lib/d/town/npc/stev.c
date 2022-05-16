#include <ansi.h>
#include <mudlib.h>
inherit SHOPKEEP;

reset(arg) {
    ::reset(arg);
    if (arg) return;
    add_object("NOBLE_OB","d/town/obj/NOBLE_OB");
    STOREROOM = "/d/town/market/a_warehouse";
    SIGN_TEXT = "Welcome to the armour store, 'help shop' for assistance";
    if(arg) return;
    set_name("stev");
    set_alias("shopkeep");
    set_type("armour");
    set_short(RED_F+"dragonkin shopkeep"+NORM);
    set_long(
      "The cunning and reputedly deadly rogue of the dragonkin merchant clan,\n"+
      "Stev, stands watch over his shop, his beady red eyes keeping close\n"+
      "watch on all patrons. He has a nasty nervous twitch that shows his\n"+
      "yellowish buck teeth, and one of his ears seems to have been badly\n"+
      "torn in combat. He dresses in black leather except for a few bits\n"+
      "of gleaming silver.\n");
    set_race("dragonkin");
    set_gender(1);
    set_level(50);
    set_half_exp(1);
    set_magic_resist(50);

}

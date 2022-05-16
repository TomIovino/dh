#include <mudlib.h>
inherit SHOPKEEP;
#include <ansi.h>

reset(arg) {
    ::reset(arg);
    if (arg) return;
    STOREROOM = "d/town/market/w_warehouse";
    add_object("NOBLE_OB","d/town/obj/NOBLE_OB");
    set_name("tuji");
    set_short(BROWN_F+"dragonkin shopkeep"+NORM);
    set_type("weapon");
    set_alias("shopkeep");
    set_long(
      "This powerfully muscled troll must be Tuji, famed weapon smith of\n"+
      "Twilite City. He wears a thick leather apron about his body, and\n"+
      "drips with sweat from hammering hot steel on his anvil. He doesn't\n"+
      "smell all that pleasant, but he does surprisingly fine metal work.\n");
    set_race("dragonkin");
    set_gender(1);
    load_spells(10,({
       "heal",
      }));
    set_level(50);
    set_half_exp(1);
    set_magic_resist(50);

}

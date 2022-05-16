#include <mudlib.h>
inherit SHOPKEEP;
#include <ansi.h>

reset(arg) {
    ::reset(arg);
    if (arg) return;
    add_object("NOBLE_OB","d/town/obj/NOBLE_OB");
    STOREROOM = "d/town/market/c_warehouse";
    set_name("deneri");
    set_type("clothing");
    set_short(MAGENTA_F+"dragonkin shopkeep"+NORM);
    set_alias("shopkeep");
    set_long(
      "Garbed lavishly in the latest styles of Twilite City, Deneri is\n"+
      "clearly a man of the world. He assists patrons of this shop with\n"+
      "a snobbish air of self-importance, and although his words are not\n"+
      "by any means intelligent, he speaks them as though every sentence\n"+
      "were the utterance of pure wisdom. Despite all this, his deep amber\n"+
      "eyes hold a harshness, and the way his hand twitches towards his\n"+
      "hilt frequently tells you this is no one to be trifled with.\n");
    set_race("dragonkin");
    set_gender(1);
    load_spells(10,({
      "reflect","barrier","deflect","heal","heal",
      }));
    set_level(50);
    set_half_exp(1);
    set_magic_resist(50);

}

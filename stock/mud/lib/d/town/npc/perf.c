#include <ansi.h>
#include <mudlib.h>
inherit SHOPKEEP;

reset(arg) {
    ::reset(arg);
    if (arg) return;
    add_object("NOBLE_OB","d/town/obj/NOBLE_OB");
    STOREROOM = "/d/town/market/g_warehouse";
    SIGN_TEXT = "Welcome to the general store, 'help shop' for assistance";
    if(arg) return;
    set_name("perf");
    set_alias("shopkeep");
    set_short(BROWN_F+"dragonkin shopkeep"+NORM);
    set_long(
      "This is Perf, a well known and prosperous merchant said to have an unusually\n"+
      "generous heart. While he makes a good profit in all dealings, he has earned\n"+
      "a well deserved reputation of fairness. He dressed in woolen clothing of the\n"+
      "highest quality in both cloth and tailoring.\n");
    set_race("dragonkin");
    set_gender(1);
    set_level(50);
    set_half_exp(1);
    set_magic_resist(50);

}

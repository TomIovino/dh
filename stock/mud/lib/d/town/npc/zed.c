#include <mudlib.h>
#include <ansi.h>

inherit SHOPKEEP;

reset(arg) {
    ::reset(arg);
    if (arg) return;
    STOREROOM = "/d/town/market/gift_warehouse";
    SIGN_TEXT = "Welcome to the gift shop, 'help shop' for assistance";
    set_name("zed");
    set_alt_name("shopkeep");
    set_alias("shopkeeper");
    set_short(B_BLUE_F+"puttering shopkeeper"+NORM);
    set_long(
      "This is Zed the Kumak Keep florist and gift shop proprietor. He looks\n"+
      "fairly cheerful, but his deep set eyes and ragged though festive clothing\n"+
      "speak of difficult times and slow business.\n");
    set_level(50);
    set_gender(1);
    set_race("half-elf");
    set_helpers(({"",({"soldier","guard"}),}));
    set_half_exp(1);
    load_spells(5,({
	"heal","exoskin","heroism","heal"
      }));
}

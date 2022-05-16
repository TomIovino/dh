#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;

reset(arg) {
    ::reset(arg);
    if(arg) return;
    add_object("girth","/d/world/obj/girth");
    add_object("robe","/d/world/obj/rrobe");
    set_name("zihweeg");
    set_alias("man");
    set_short(RED_F+"red robed man"+NORM);
    set_long(
      "Before you stands the ancient red mage, Zihweeg.\n");
    set_race("human");
    set_gender(1);
    load_chat(2,({
        "Zihweeg mumbles about object structures.\n",
        "*chuckle",
      }));
    set_tactic("casting");
    set_helpers(({ "", ({ "amylaar", "bubbs", }), }));
    set_gender(1);
    load_spells(10,({
      "heal","blast","lance","storm","heal",
      }));
/*
    set_train_level(1);
    set_stat_int(35);
    set_stat_str("intelligence");
    set_class_list(({
	"spells", ({
             "spike",
	}),
      }));
*/
    set_level(90);
    set_magic_resist(25);
}

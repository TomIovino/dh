#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;

reset(arg) {
    ::reset(arg);
    if(arg) return;
    add_object("boots","/d/world/obj/boots");
    add_object("robe","/d/world/obj/wrobe");
    set_name("zihweeg");
    set_alias("man");
    set_short(B_WHITE_F+"white robed man"+NORM);
    set_long(
      "Before you stands the ancient white mage, Amylaar.\n");
    set_race("human");
  set_helpers(({ "", ({ "bubbs", "zihweeg", }), }));
    set_gender(1);
    load_chat(2,({
        "Amylaar mumbles about structered protocol.\n",
        "*ponder",
      }));
    set_tactic("casting");
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

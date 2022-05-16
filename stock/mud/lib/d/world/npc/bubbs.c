#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;

reset(arg) {
    ::reset(arg);
    if(arg) return;
    add_object("gloves","/d/world/obj/gloves");
    add_object("robe","/d/world/obj/brobe");
    set_name("bubbs");
    set_alias("man");
    set_short(GREY_F+"black robed man"+NORM);
    set_long(
      "Before you stands the ancient black mage, Bubbs.\n");
    set_race("human");
  set_helpers(({ "", ({ "amylaar", "zihweeg", }), }));
    set_gender(1);
    load_chat(2,({
        "Bubbs mumbles about recursive standards.\n",
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

#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;

reset(arg) {
    ::reset(arg);
    if(arg) return;
    add_object("robe","/d/world/obj/robes");
    set_name("robin");
    set_alias("woman");
    set_short(RED_F+"white robed old woman"+NORM);
    set_long(
      "Before you stands the ancient priest, Robin, who is said\n"+
      "to be older than the land itself. Although her features are\n"+
      "weathered with the passing of ages, something about her is\n"+
      "ageless, perhaps it's her faith.\n");
    set_race("human");
    set_gender(2);
    load_chat(2,({
   "Robin says: Don't play with mother nature.\n",
   "*ponder",
      }));
    set_tactic("defensive");
    load_spells(10,({
      "heal","blast","spike","storm","heal",
      }));
/*
    set_train_level(1);
    set_stat_int(35);
    set_stat_str("wisdom");
    set_class_list(({
	"spells", ({
          "heal",
	}),
      }));
*/
    set_level(90);
    set_magic_resist(80);
}

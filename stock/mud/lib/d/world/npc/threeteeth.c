#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;

reset(arg) {
    ::reset(arg);
    if(arg) return;
    add_object("bracers","/d/world/obj/bracers");
    set_name("threeteeth");
    set_alias("ogre");
    set_short(RED_F+"ogre magi"+NORM);
    set_long(
      "Before you stands a great elder of the race of ogres,\n"+
      "whose two top and one bottom tooth marks his namesake.\n"+
      "Through the long years and weathered days, Threeteeth has\n"+
      "become quite a master of combat.\n");
    set_race("ogre");
    set_gender(1);
    load_chat(2,({
   "Threeteeth says: Me have good fire, you get warm quick.\n",
   "Threeteeth says: Me like chicken.\n"
      }));
    set_tactic("offense");
    set_gender(1);
    load_spells(10,({
      "heal","blast","spike","storm","heal",
      }));
/*
    set_train_level(1);
    set_stat_int(35);
    set_stat_str("fight");
    set_class_list(({
	"skills", ({
        "offensive tactics",
	}),
      }));
*/
    set_level(90);
    set_magic_resist(15);
}

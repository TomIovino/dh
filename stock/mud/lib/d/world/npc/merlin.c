#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;

reset(arg) {
    ::reset(arg);
    if(arg) return;
    add_object("spectacles","/d/world/obj/spectacles");
    set_name("merlin");
    set_alias("man");
    set_short(RED_F+"red robed old man"+NORM);
    set_long(
      "Before you stands the ancient mage, Merlin, who is said\n"+
      "to be older than the land itself. Most of the greatest of all\n"+
      "mages were at one time his students. He is even said to have\n"+
      "taught the Archmage Amylaar at one time.\n");
    set_race("human");
    set_gender(1);
    load_chat(2,({
   "Merlin says: The greatest power is the power of light.\n",
   "Merlin says: Amylaar was my finest apprentice.\n"
        "*chuckle","*hmm",
      }));
    set_tactic("casting");
    set_gender(1);
    load_spells(10,({
      "heal","blast","spike","storm","heal",
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

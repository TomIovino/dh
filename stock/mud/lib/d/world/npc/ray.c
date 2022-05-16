#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;

reset(arg) {
    ::reset(arg);
    if(arg) return;
    add_object("redscimitar","/d/world/obj/redscimitar");
    add_object("bluescimitar","/d/world/obj/bluescimitar");
    set_name("ray");
    set_alias("ranger");
    set_short(GREEN_F+"old elven ranger"+NORM);
    set_long(
      "Before you stands the ancient elven ranger, Ray, who through\n"+
      "the centuries managed to watch over the great forest. His hair\n"+
      "is silver but he looks like a master of combat.\n");
    set_race("elf");
    set_gender(1);
    load_chat(2,({
   "Ray says: This used to be the pass to the Dragonmount.\n",
   "Ray says: Shelia may be able to help you find your way north.\n",
      "*exoskin","*heroism","*bless","*frenzy",
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
          "dual wield","track",
	}),
      }));
*/
    set_level(90);
    set_magic_resist(80);
}

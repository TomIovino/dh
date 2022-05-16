#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;

reset(arg) {
    ::reset(arg);
    if(arg) return;
    set_name("bubba");
    add_object("club","/d/world/obj/club");
    add_object("shield","/d/world/obj/shield");
    set_alt_name("warrior");
    set_alias("orc");
    set_short(CYAN_F+"orcish warrior"+NORM);
    set_long(
      "This is Bubba, the biggest, meatiest, thickest Orc you have ever seen.\n"+
      "Bubba also has plenty of scars from previous fights that have made him\n"+
      "even meaner looking. This is perhaps one Orc you shouldn't pick on.\n");
      set_race("orc");
    set_helpers(({ "", ({ "shao", }), }));
    set_gender(1);
    load_chat(1, ({ "Bubba says: huhuhuhuh\n",
                    "Bubba says: uh huh\n",
               }));
/*
    set_train_level(9);
    set_stat_int(35);
    set_stat_str("strength");
    set_class_list(({
	"skills", ({
          "shield block","shield crush",
	}),
      }));
*/
    load_spells(10, ({ "heal","heal","heal", }));
    set_level(90);
    set_magic_resist(0);
}

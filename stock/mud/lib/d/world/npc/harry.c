#include <ansi.h>
#include <mudlib.h>
inherit MONSTER;

reset(arg) {
    ::reset(arg);
    if(arg) return;
    add_object("longsword","d/world/obj/longsword");
    add_object("fullplate","d/world/obj/fullplate");
    set_name("harry");
    set_alias("large");
    set_short(MAGENTA_F+"large giant"+NORM);
    set_long(
      "This is without question the legendary Harry, most noble and most gentle\n"+
      "of the race of giants. He stands here with a helpful expression on his\n"+
      "face, ready to teach to those who would listen the arts of battle, but\n"+
      "not for the glory of battle. He teaches so that his students may better\n"+
      "defend themselves and others from those who would force battle on them.\n");
    set_race("giant");
    set_gender(1);
/*
    set_train_level(9);
    set_stat_int(30);
    set_stat_str("constitution");
    set_class_list(({
 	"skills", ({
          "multi attack","block",
       	  "critical hit","multi opponents",
           "enhanced damage",
          "offensive tactics",
	}),
      }));
*/
    set_level(90);
    set_magic_resist(10);

}


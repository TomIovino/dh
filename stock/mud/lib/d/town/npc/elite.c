
#include <mudlib.h>
inherit MONSTER;
#include <ansi.h>
#define PATH "/d/town/obj/royal/"
int u,e;
#define CLONE  ({ "amulet","ankles","armour","belt","boots","cloak",\
                  "floater","gloves","helm","pants","ring","shield",\
                  "sleeves","wrists","mask", })

int o             = random(sizeof(CLONE));
reset(arg){
    ::reset(arg);
    if(arg) return;
if(!random(10))
    add_object(CLONE[o],PATH+CLONE[o]);
    set_gender(1+random(2));
    set_level(75);
    set_magic_resist(25);
    set_long(
      capitalize(this_object()->query_pronoun())+" is a well trained and battle hardened Guard of the Watch\n"+
      "sworn to protect the great city of Twilite from all invaders.\n"+
      "Dressed in fine chainmail covered with a gleaming white tabard\n"+
      "bearing the crest of Twilite City, this guard is clearly a\n"+
      "dedicated professional soldier.\n");
    add_money(50);
    load_spells(10, ({
     "fangs","exoskin","wings","claws",
     "heal","exoskin","holy ground","blast",
     "storm","heal","curse","dispel",
    }));
    set_half_exp(1);
    set_name("guard");
    set_alias("elite");
    set_wander(50, 30, 0, "/d/town/castle");
    set_short(BLUE_F+"twilite dragonkin elite-guard"+NORM);
    set_race("dragonkin");
    set_magic_resist(50);
    set_helpers(({ "", ({ "guard", }) , }));
}


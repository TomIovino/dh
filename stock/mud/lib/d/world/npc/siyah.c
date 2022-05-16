#include <ansi.h>
#include <mudlib.h>
inherit MONSTER;

reset(arg) {
    ::reset(arg);
    if (arg) return;
    if(arg) return;
    add_object("cloak","/d/world/obj/cloak");
    set_name("siyah");
    set_alias("ranger");
    set_short(BROWN_F+"elven ranger"+NORM);
    set_race("elf");
    set_helpers(({ "", ({ "greyelf", }), }));
    set_long(
    "This is Siyah Yanamali, he is said to have seen many, many things.\n"+
    "His darkened skin is weathered like leather from the many suns he has\n"+
    "seen. He has walked across this realm uphill both ways.\n");
    set_gender(1);
    load_chat(1, ({ 
     "Siyah enscribes a rune of power.\n",
     "*heal","*reflect","*barrier","*avenger","*defender","*bless",
     "*health rune",
    }));
    load_spells(10, ({ "heal","jolt","spike" }));
    set_level(90);
    set_magic_resist(10);
}


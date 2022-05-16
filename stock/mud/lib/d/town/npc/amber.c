#include <ansi.h>
#include <mudlib.h>
inherit BANKKEEP;

reset(arg) {
    ::reset(arg);
    if(arg) return;
    add_object("NOBLE_OB","d/town/obj/NOBLE_OB");
    set_name("amber");
    set_alias("banker");
    set_short(B_RED_F+"dragonkin banker"+NORM);
    set_long(
   "This is no doubt the well known banker, Amber, who they say has a hand\n"+
   "in all business in and around Twilite, legal and otherwise. She seems\n"+
   "a pleasant person, with a charming smile and friendly way about her. Yet\n"+
   "there is a hardness behind those sparkling blue eyes that makes it clear\n"+
   "she won't tolerate any arguments regarding business or accounts.\n");
    set_race("dragonkin");
    set_gender(2);
    load_spells(20, ({ "heal", }));
    set_level(50);
    set_half_exp(1);
    set_magic_resist(50);
}


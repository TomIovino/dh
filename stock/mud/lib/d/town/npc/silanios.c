#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;
// #define WCLONE ({  })

#define CLONE  ({ "amulet","armour","belt", "boots","cloak","gauntlets",\
                  "greaves","helm","jewel","quest","ring","shield",\
                  "vambraces", "ankles","wrists","mask" })

#define   PATH "/d/town/obj/king/"

int o = random(sizeof(CLONE));
// int w = random(sizeof(WCLONE));
reset (arg){
    ::reset(arg);
    if(arg) return;
    set_alignment("divinity");
    set_name("silanios");
    set_alias("king");
    set_short(BOLD"dragonkin king"NORM);
    set_long(
"Like all of his kind he is a creation of powerful magic.\n"+
"He stands like a human but slightly taller and stronger.\n"+
"He doesn't have skin like most humanoids either, but rather\n"+
"is covered with glistening silver scales.\n");
    set_race("dragonkin");
    set_gender(1);
    set_level(100);
/*
    add_object("bag",PATH+"quest");
*/
  if(!random(10))
    add_object(CLONE[o],PATH+CLONE[o]);
    set_helpers(({ "", ({ "dragonkin", }) , }));
  load_chat(5, ({
  "*ponder","*hmm","*heal","*exoskin","*bless","*barrier","*fangs",
  "*heal","*reflect","*exoskin","*bless","*avenger","*defender","*claws",
  "King Silanios says, my ancient line extends directly from Crimsonesti.\n",
}));

  load_spells(10, ({ "fangs","exoskin",
   "heal","exoskin","holy ground","blast",
   "storm","heal","curse","dispel",
  }));     
   add_money(random(250)+250);
    set_magic_resist(50);
/*
    set_dead_ob(this_object()); 
*/
}


death_2(){
/*
 move_object(clone_object("w/ash/dragon/fifth/quest"),this_player()); 
 say("You have defeated the dragonkin king.\n");
 write(this_player()->query_alt_name()+" has defeated the dragonkin king.\n");
*/
}


#define TP this_player()

#include <mudlib.h>
#include <ansi.h>

inherit MONSTER;

reset (arg){
   ::reset(arg);
   if(arg) return;
  set_name("golem");
  set_alias("stone");
  set_short(YELLOW_F+"amber golem");
  set_long("A golem made of amber gemstone.\n");
  set_special_chance(10);
  set_special_dam(5);
  set_special_mess1("Golem smashes you with his fist!\n");
  set_special_mess2("Golem smashes you with his fist!\n");
  set_special_type("crush");
  set_level(20);
  load_spells(10, ({ "lance" }));
  set_magic_resist(25);
  add_object("amulet","w/ash/shadow/obj/amulet");
  add_object("armor","w/ash/shadow/obj/armour");
  add_object("belt","w/ash/shadow/obj/belt");
  add_money(random(200)+100);
/*
  set_remort("earth",2);
*/

}



death_2() {
  say(NORM+YELLOW_F+"The golem screams a death cry.\n");
  write(NORM+YELLOW_F+"You have killed the golem.\n");
  return 0;
}



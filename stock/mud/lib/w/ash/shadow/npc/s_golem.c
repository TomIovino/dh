
#define TP this_player()

#include <mudlib.h>
#include <ansi.h>

inherit MONSTER;

reset (arg){
   ::reset(arg);
   if(arg) return;
  set_name("golem");
  set_alias("stone");
  set_short(B_BLUE_F+"sapphire golem");
  set_long("A golem made of sapphire gemstone.\n");
  set_special_chance(10);
  set_special_dam(5);
  set_special_mess1("Golem smashes you with his fist!\n");
  set_special_mess2("Golem smashes you with his fist!\n");
  set_special_type("crush");
  set_level(20);
  load_spells(10, ({ "lance" }));
  set_magic_resist(25);
  add_object("helm","w/ash/shadow/obj/helm");
  add_object("gloves","w/ash/shadow/obj/gloves");
  add_object("leggings","w/ash/shadow/obj/leggings");
  add_money(random(200)+100);
/*
  set_remort("water",2);
*/

}



death_2() {
  say(B_BLUE_F+"The golem screams a death cry.\n");
  write(B_BLUE_F+"You have killed the golem.\n");
  return 0;
}



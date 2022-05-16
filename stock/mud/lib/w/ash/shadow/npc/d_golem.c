
#define TP this_player()

#include <mudlib.h>
#include <ansi.h>

inherit MONSTER;

reset (arg){
   ::reset(arg);
   if(arg) return;
  set_name("golem");
  set_alias("stone");
  set_short(B_WHITE_F+"diamond golem");
  set_long("A golem made of diamond gemstone.\n");
  set_special_chance(10);
  set_special_dam(5);
  set_special_mess1("Golem smashes you with his fist!\n");
  set_special_mess2("Golem smashes you with his fist!\n");
  set_special_type("crush");
  set_level(20);
  load_spells(10, ({ "lance" }));
  set_magic_resist(25);
  add_object("boots","w/ash/shadow/obj/boots");
  add_object("cloak","w/ash/shadow/obj/cloak");
  add_object("floating","w/ash/shadow/obj/floating");
  add_money(random(200)+100);
/*
  set_remort("air",2);
*/

}


death_2() {
  say(B_WHITE_F+"The golem screams a death cry.\n");
  write(B_WHITE_F+"You have killed the golem.\n");
  return 0;
}



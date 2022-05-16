
#define TP this_player()

#include <mudlib.h>
#include <ansi.h>

inherit MONSTER;

reset (arg){
   ::reset(arg);
   if(arg) return;
  set_name("golem");
  set_alias("stone");
  set_short(NORM+"m"GREY_F"ar"NORM"bl"GREY_F"e go"NORM"le"GREY_F"m"NORM);
  set_long("A golem made of solid marble.\n");
  set_special_chance(10);
  set_special_dam(5);
  set_special_mess1("Golem smashes you with his fist!\n");
  set_special_mess2("Golem smashes you with his fist!\n");
  set_special_type("crush");
  set_level(20);
  load_spells(10, ({ "lance" }));
  set_magic_resist(25);
  add_object("mask","w/ash/shadow/obj/mask");
  add_object("wrists","w/ash/shadow/obj/wrists");
  add_object("ankles","w/ash/shadow/obj/ankles");
  set_skill("shield block",1);
  add_money(random(200)+100);
/*
  set_remort("fire",2);
*/

}

init(){
   ::init();
add_action("down","d");
add_action("down","down");
}

down(){
   write("Marble golem stands in your way!\n");
   return 1;
}


death_2() {
  say(B_RED_F+"The golem screams a death cry.\n");
  write(B_RED_F+"You have killed the golem.\n");
  return 0;
}


    

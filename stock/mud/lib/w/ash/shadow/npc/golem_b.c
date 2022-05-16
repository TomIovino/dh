
#define TP this_player()

#include <mudlib.h>
#include <ansi.h>

inherit MONSTER;

reset (arg){
   ::reset(arg);
   if(arg) return;
  set_name("golem");
  set_alias("crystal");
  set_short(B_WHITE_F+"cr"B_RED_F"ys"YELLOW_F"ta"BLUE_F
  "l go"B_WHITE_F"le"B_RED_F"m"NORM);
  set_long("A golem made of solid crystal.\n");
  set_special_chance(10);
  set_special_dam(5);
  set_special_mess1("Golem smashes you with his fist!\n");
  set_special_mess2("Golem smashes you with his fist!\n");
  set_special_type("crush");
  set_level(20);
  load_spells(10, ({ "lance" }));
  set_magic_resist(25);
  add_object("claws","w/ash/shadow/obj/claws");
  set_skill("shield block",1);
  add_money(random(400)+100);
/*
  set_remort("air",1);
  set_remort("water",1);
  set_remort("fire",1);
  set_remort("earth",1);
*/

}
/*
init(){
   ::init();
add_action("down","d");
add_action("down","down");
}

down(){
   write("Marble golem stands in your way!\n");
   return 1;
}
*/

death_2() {
  say(B_RED_F+"The golem screams a death cry.\n");
  write(B_RED_F+"You have killed the golem.\n");
  return 0;
}


    

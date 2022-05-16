
#include <mudlib.h>
#include <ansi.h>
#include "../element.h"
inherit CONTAINER;

void reset(status arg) {
  string mask,weapon,armour,shield,boots,bag,water,food,beer,torch;
  mask = PATH+"obj/1mask";
  weapon = PATH+"obj/1weapon";
  armour = PATH+"obj/1armour";
  shield = PATH+"obj/1shield";
  boots =  PATH+"obj/1boots";
  bag =    "inherit/objs/bag";
  beer =   "inherit/objs/beer";
  water =  "inherit/objs/water";
  food =   "inherit/objs/food";
  torch =  "inherit/objs/torch";
if(!present("weapon"))
  move_object(clone_object(weapon),this_object());
if(!present("mask"))
  move_object(clone_object(mask),this_object());
if(!present("armour"))
  move_object(clone_object(armour),this_object());
if(!present("shield"))
  move_object(clone_object(shield),this_object());
if(!present("boots"))
  move_object(clone_object(boots),this_object());
if(!present("bag"))
  move_object(clone_object(bag),this_object());
if(!present("beer"))
  move_object(clone_object(beer),this_object());
if(!present("food"))
  move_object(clone_object(food),this_object());
if(!present("water"))
  move_object(clone_object(water),this_object());
if(!present("torch"))
  move_object(clone_object(torch),this_object());

  if(arg) return;
  set_name("chest");

  set_short(COLOR+"donation chest"NORM);
  set_long("\
An Ornate chest to hold donations for the needy.\n\
  ");
  set_max_weight(10000);
  set_value(0);
  set_weight(100000);
}

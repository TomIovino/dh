#define CLONE_OBJ(NN,FF) if(!present(NN,this_object())){ move_object(clone_object(FF),this_object()); }

#include <mudlib.h>
#include <ansi.h>
inherit CONTAINER;

void reset(status arg) {
  string weapon,armour,shield,boots,bag,beer,torch;
  weapon = "d/town/obj/slash";
  armour = "d/town/obj/armour";
  shield = "d/town/obj/shield";
  boots = "d/town/obj/boots";
  bag = "inherit/objs/bag";
  beer = "inherit/objs/beer";
  torch = "inherit/objs/torch";
if(!present("weapon"))
  move_object(clone_object(weapon),this_object());
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
if(!present("torch"))
  move_object(clone_object(torch),this_object());

  if(arg) return;
  set_name("chest");

  set_short(YELLOW_F+"ornate donation chest"NORM);
  set_long("\
An Ornate chest to hold donations for the needy.\n\
  ");
  set_max_weight(10000);
  set_value(0);
  set_weight(100000);
}

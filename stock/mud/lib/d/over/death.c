#include <ansi.h>
inherit "inherit/monster";

reset(arg) {
  ::reset(arg);
  if(arg) return;

  set_name("death");
  set_short(RED_F+"Death, dressed in black robes"+NORM);
  set_long(
"The dark skelton skeleton figure of Death\n");
  set_race("immortal");
  set_gender(1);
  set_level(200);
  move_object(clone_object("d/over/deathcloak"),this_object());
  init_command("wear cloak");
  move_object(clone_object("d/over/deathweapon"),this_object());
  init_command("wield scythe");
  set_ac(50);
  set_magic_resist(50 + random(50));
}

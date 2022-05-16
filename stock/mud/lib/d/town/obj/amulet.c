#include <ansi.h>
#include <mudlib.h>

inherit ARMOUR;

reset(arg) {
  if(arg) return;
  set_name("collar");
  set_short(BROWN_F"soft leather collar"+NORM);
  set_long(BROWN_F+
  "A simple leather collar made of leather it offers some\n"+
  "protection to the neck.\n");
  set_type("amulet");
  set_ac(4);
  set_weight(4); 
  set_value(1000);
}


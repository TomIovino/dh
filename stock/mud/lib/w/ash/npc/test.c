#include <mudlib.h>
#include <ansi.h>
inherit MONSTER;

reset(arg) {
  ::reset(arg);
  if(arg) return;

  set_name("test");
  set_aggressive(0);
  set_short(BOLD+BLUE_F+"test elemental"+NORM);
  set_alias("water");
  set_long("\
A playful and seemingly unthreatening creature is invisible while in\n\
the water.  As she rises out of the water, her form appears.  Hovering\n\
over the water, this voluptuous young female has long, golden hair and\n\
sparkling green eyes.  Her voice and her eyes entrance you.\n\
");
  set_long("elemental");
  set_race("human");
    load_spells(30, ({
     "blast",
    }));
  set_gender(0);
  set_level(50);
}
init(){
  ::init();
  add_action("swim","dive");
  }
  swim(){
   write("The waterweird blocks your path!\n");
   return 1;
  }

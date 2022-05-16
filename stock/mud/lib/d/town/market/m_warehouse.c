
#define PATH "/inherit/objs/"
#include <mudlib.h>
inherit ROOM;
int default_objects = 1;

void set_no_default_items() { default_objects = 0; }

reset(arg) {
if(default_objects) {
  add_object("mring","/d/town/obj/clothing/wring_male");
  add_object("fring","/d/town/obj/clothing/wring_fem");
  add_object("perfume",PATH+"perfume");
  add_object("mango",PATH+"mango");
  add_object("berry",PATH+"berry");
  add_object("mirror",PATH+"mirror");
  add_object("pearl",PATH+"pearl");
  add_object("diamond",PATH+"diamond");
  add_object("sapphire",PATH+"sapphire");
  add_object("ruby",PATH+"ruby");
  add_object("sulphur",PATH+"sulphur");
  add_object("rock",PATH+"rock");
  add_object("gem", PATH+"gem");
  add_object("rope", PATH+"rope");
  add_object("pearl",PATH+"pearl");
  add_object("ashes",PATH+"ashes");
  add_object("gem",PATH+"gem");
  add_object("compass",PATH+"compass");
  add_object("ankh","/w/ash/base/art/q_ankh");
}

  if(arg) return;
  set_short("Storeroom");
  set_long("\
This is the storeroom for shop.\n");
  set_light(2);
  set_exits(({
   "/secure/void","out",
  }));
}

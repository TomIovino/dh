
#define PATH "/d/town/obj/clothing/"
#include <mudlib.h>
inherit ROOM;

reset(arg) {
// leggings
        add_object("blue leggings",PATH+"leg_blue");
        add_object("cyan leggings",PATH+"leg_cyan");
        add_object("green leggings",PATH+"leg_green");
        add_object("magenta leggings",PATH+"leg_magenta");
        add_object("red leggings",PATH+"leg_red");
        add_object("white leggings",PATH+"leg_white");
        add_object("yellow leggings",PATH+"leg_yellow");
// cloak
        add_object("blue cloak",PATH+"cloak_blue");
        add_object("cyan cloak",PATH+"cloak_cyan");
        add_object("green cloak",PATH+"cloak_green");
        add_object("magenta cloak",PATH+"cloak_magenta");
        add_object("red cloak",PATH+"cloak_red");
        add_object("white cloak",PATH+"cloak_white");
        add_object("yellow cloak",PATH+"cloak_yellow");
// tunic
        add_object("blue tunic",PATH+"tunic_blue");
        add_object("cyan tunic",PATH+"tunic_cyan");
        add_object("green tunic",PATH+"tunic_green");
        add_object("magenta tunic",PATH+"tunic_magenta");
        add_object("red tunic",PATH+"tunic_red");
        add_object("white tunic",PATH+"tunic_white");
        add_object("yellow tunic",PATH+"tunic_yellow");

    if(arg) return;
    set_short("Storeroom");
    set_long("\
This is the storeroom for shop.\n");
    set_light(2);
    set_exits(({
	"/secure/void","out",
      }));
}

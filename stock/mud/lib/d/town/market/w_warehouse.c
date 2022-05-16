
#define PATH "/d/town/obj/"
#include <mudlib.h>
inherit ROOM;

reset(arg) {
    add_object("slash",PATH+"slash");
    add_object("thrust",PATH+"thrust");
    add_object("cleave",PATH+"cleave");
    add_object("crush",PATH+"crush");
    add_object("longsword",PATH+"slash_2");
    add_object("spear",PATH+"thrust_2");
    add_object("polearm",PATH+"cleave_2");
    add_object("staff",PATH+"crush_2");
    if(arg) return;
    set_short("Storeroom");
    set_long("\
This is the storeroom for shop.\n");
    set_light(2);
    set_exits(({
	"/secure/void","out",
      }));
}

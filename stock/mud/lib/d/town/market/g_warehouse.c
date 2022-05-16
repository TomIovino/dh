
#define PATH "/d/town/obj/"
#include <mudlib.h>
inherit ROOM;

reset(arg) {
	add_object("bag","inherit/objs/bag");
	add_object("torch","inherit/objs/torch");
    if(arg) return;
    set_short("Storeroom");
    set_long("\
This is the storeroom for shop.\n");
    set_light(2);
    set_exits(({
	"/secure/void","out",
      }));
}

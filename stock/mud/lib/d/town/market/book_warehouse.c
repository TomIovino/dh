
#define PATH "/d/town/obj/"
#include <mudlib.h>
inherit ROOM;

reset(arg) {
    if(arg) return;
    set_short("Storeroom");
    set_long("\
This is the storeroom for shop.\n");
    set_light(2);
    set_exits(({
	"/secure/void","out",
      }));
}

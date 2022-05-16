
#define PATH "/examples/"
#include <mudlib.h>
inherit ROOM;
int default_objects = 1;

void set_no_default_items() { default_objects = 0; }

reset(arg) {

    if(default_objects) {
    }

    if(arg) return;
    set_short("Storeroom");
    set_no_summon(1);
    set_no_save(1);
    set_long("\
This is the storeroom for shop.\n");
    set_light(2);
    set_exits(({
	"/secure/void","out",
      }));
}


#define PATH "/d/town/obj/"
#include <mudlib.h>
inherit ROOM;

reset(arg) {
    add_object("ankles",PATH+"ankles");
    add_object("wrists",PATH+"wrists");
    add_object("armour",PATH+"armour");
    add_object("belt",PATH+"belt");
    add_object("boots",PATH+"boots");
    add_object("cloak",PATH+"cloak");
    add_object("floating",PATH+"floating");
    add_object("gloves",PATH+"gloves");
    add_object("helm",PATH+"helm");
    add_object("pants",PATH+"pants");
    add_object("ring",PATH+"ring");
    add_object("shield",PATH+"shield");
    add_object("sleeves",PATH+"sleeves");
    if(arg) return;
    set_short("Storeroom");
    set_long("\
This is the storeroom for shop.\n");
    set_light(2);
    set_exits(({
	"/secure/void","out",
      }));
}

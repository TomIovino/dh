
#define PATH "/d/town/obj/gift/"
#include <mudlib.h>
inherit ROOM;

reset(arg) {
    add_object("bear",PATH+"bear");
    add_object("beer",PATH+"beer");
    add_object("chocolates",PATH+"box");
    add_object("bracelet",PATH+"brace");
    add_object("black rose",PATH+"brose");
    add_object("bunny",PATH+"bunny");
    add_object("black bouquet",PATH+"dbrose");
    add_object("red bouquet",PATH+"drrose");
    add_object("claddagh ring",PATH+"ring1");
    add_object("engagement ring",PATH+"ring3");
    add_object("red rose",PATH+"rrose");
    if(arg) return;
    set_short("Storeroom");
    set_long("\
This is the storeroom for shop.\n");
    set_light(2);
    set_exits(({
	"/secure/void","out",
      }));
}

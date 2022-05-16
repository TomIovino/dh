#include <mudlib.h>
inherit ROOM;

#define TP this_player() /* im to lazy */

reset(arg) {
    ::reset(arg);
    if(!present("Death"))
	move_object(clone_object("d/death/npc"),this_object());
    if(arg) return;
    set_short("Somewhere between heaven and hell");
    set_long("Somewhere between heaven and hell. Type 'church' to go "+
      "to your church.\n");
    set_light(2);
    set_items(({ }));
    set_no_fight(1);
}

init() {
    ::init();
    add_action("do_church", "church");
    add_action("no_stealing","steal");
}

do_church() {
      this_player()->church();
}


no_stealing() {
    write("Thou shall not steal here.\n");
    return 1;
}


int query_prop(string str) {
    if(str == "Nulmagic") return 1;
    return 0;
}

int query_property(string str) {
    return query_prop(str);
}

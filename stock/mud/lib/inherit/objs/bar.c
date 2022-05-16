/* a bag */

#include <mudlib.h>
inherit CONTAINER;

void reset(status arg) {
    if(arg) return;
    set_name("bar");
    set_weight(99999);
    set_long("\
An oak and walnut bar. Drinks are served and can be kept here.\n\
");
    set_max_weight(500);
    set_value(12);
}

init(){
    ::init();
    add_action("look", "l");
    add_action("look", "examine");
    add_action("look", "exa");
    add_action("look", "look");
}

look(str){
    object *ob;  int i;
    if(str == "at bar" || str == "bar"){
	ob = all_inventory(this_object());
	write("A black bar made from walnut and oak. The bar was made to hold the\n"+
	  "drinks of the patrons. It currently contains:\n");
	for(i = 0; i < sizeof(ob); i++){
	    write(ob[i]->query_short()+"\n"); }
	return 1; }
    return 0; }

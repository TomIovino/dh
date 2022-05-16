/* a peace function which works properly. -Belial. 3-7-96 */

status main() {
    object *ob;
    int i;
    ob=all_inventory(environment(this_player()));
    for(i=0;i<sizeof(ob);i++) {
	if(living(ob[i])) {
	    ob[i]->stop_fight();
	    ob[i]->stop_fight();
	}
    }
    write("You create peace.\n");
    say(capitalize(this_player()->query_real_name())+" creates peace.\n");
    return 1;
}

void help() {
    write("useage: peace\n");
    write("Stops all battle in that room.\n");
    return;
}

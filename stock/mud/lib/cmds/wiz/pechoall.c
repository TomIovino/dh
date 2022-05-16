/* pechoall- echoall to just players so that it doesnt intefere with */
/* on line wizards coding, writing posts, etc etc -Belial. 4-3-96 */

status main(string str) {
    object *players;
    int i;
    if(!str) {
	write("Echo what?\n");
	return 1;
    }
    players=users();
    for(i=0;i<sizeof(players);i++) {
	if(players[i]->query_security_level() < 1) {
	    tell_object(players[i],str+"\n");
	}
    }
    write("You echo to players: "+str+"\n");
    return 1;
}

void help() {
    write("useage: pechoall <whatever>\n");
    return;
}

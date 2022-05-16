/* farget. can get any object regardless where it is on the mud */
/* -Belial. 3-5-96 */
status main(string str){
    object thing;
    if(!str) {
	write("farget what?\n");
	return 1;
    }
    thing=find_object(str);
    if(!thing) {
	write("Can't find that.\n");
	return 1;
    }
    move_object(thing,this_player());
    write("Got it.\n");
    return 1;
}

void help() {
    write("useage: farget <filename>\n");
    return;
}

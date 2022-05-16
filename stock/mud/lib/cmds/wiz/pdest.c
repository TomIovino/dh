/* Powerdest. Can destruct any object, including invisible objects */
/* regardless where the person is on the mud */
/* -Belial. 3-5-96 */
status main(string str) {
    object thing;
    if(!str) {
	write("Usage: pdest <object>\n");
	return 1;
    }
    thing=find_object(str);
    if(!thing) {
	write(str+" not found.\n");
	return 1;
    }
    destruct(thing);
    write("Power dested it.\n");
    return 1;
}

void help() {
    write("usage: pdest <filename>\n");
    return;
}

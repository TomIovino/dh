/*
 * Written by Darou@Dune
 */

#include <udp.h>

#define IBOARD_HOST	"dune"
#define COMMAND		"CMD"

object who;
string messages,new_hd,new_body;

 
id(str) {
    return str == "inetmud bulletin board" || str == "board" || str == "bulletin board";
}

long() {
    string err;

    if (err = INETD->send_udp(IBOARD_HOST, ([
	REQUEST: "iboard",
	SENDER: this_player()->query_real_name(),
	COMMAND: "long"
    ]), 1))
	write(err);
    else
	write("Retrieving board information.\n");
}

short() {
    return ("The Dune's intermud bulletin board");
}

get() {
    write("It is secured to the ground.\n");
    return 0;
}

init() {
    add_action("new", "note");
    add_action("read", "read");
}

reset(arg) {
    if (!arg)
	messages="";
}

new(hd) {
    if (!hd)
	return 0;
    if (who && environment(who) == environment(this_object())) {
	write(who->query_name() + " is busy writing.\n");
	return 1;
    }

    if (strlen(hd) > 50) {
	write("Too long header to fit the paper.\n");
	return 1;
    }
    new_hd = hd;
    input_to("get_body");
    write("Give message, and terminate with '**'.\n");
    write("]");
    new_body = "";
    return 1;
}

get_body(str) {
    if (str == "**") {
	new_hd = new_hd + "(" + call_other(this_player(), "query_name") + "@"+ LOCAL_NAME +
	    ", " + extract(ctime(time()), 4, 9) + ")";
	messages = messages + new_hd + ":\n**\n" + new_body + "\n**\n";
	new_body = 0;
	new_hd = 0;
	if (str = INETD->send_udp(IBOARD_HOST, ([
	    REQUEST: "iboard",
	    SENDER: this_player()->query_real_name(),
	    COMMAND: "new",
            DATA: messages
	]), 1))
	    write(str);
	else
	    write("Note transmitted.\n");
        messages="";
	who = 0;
	return;
    }
    new_body = new_body + str + "\n";
    write("]");
    input_to("get_body");
}

read(str) {
    string hd, body, rest;
    int i, tmp;

    if (str == 0 ||
    (sscanf(str, "%d", i) != 1 && sscanf(str, "note %d", i) != 1))
	return 0;

    if (str = INETD->send_udp(IBOARD_HOST, ([
	REQUEST: "iboard",
	SENDER: this_player()->query_real_name(),
	COMMAND: "read",
        DATA: str
    ]), 1))
	write(str);
    else
	write("Retrieving message.\n");
    return 1;
}

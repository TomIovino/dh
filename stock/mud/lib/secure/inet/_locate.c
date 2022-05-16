#include <udp.h>

#define FOUND		"fnd"
#define USER		"user"
#define VERBOSE		"vbs"

#define V_SUCCESS	1
#define V_NO_TIME_OUTS	2

int cmd_locate(string arg) {
    mapping hosts;
    string *arr;
    int verbose, i;

    notify_fail("Usage: locate [-s|-t] <player>\n");
    if (!arg)
	return 0;
    if (sizeof(arr = explode(arg, " ")) > 1) {
	switch(arr[0]) {
	    case "-s":
		verbose = V_SUCCESS;
		break;
	    case "-t":
		verbose = V_NO_TIME_OUTS;
		break;
	    default:
		return 0;
	}
	arg = arr[1];
    }
    arg = lower_case(arg);
    for(i = sizeof(arr = m_indices(hosts = INETD->query("hosts"))); i--; ) {
	/* Don't send it to ourselves and check that the receiver
	 * recognizes "locate" requests. */
	if (arr[i] != lower_case(LOCAL_NAME) &&
	(member_array("*", hosts[arr[i]][HOST_COMMANDS]) != -1 ||
	member_array("locate", hosts[arr[i]][HOST_COMMANDS]) != -1))
	    INETD->send_udp(arr[i], ([
		REQUEST: "locate",
		SENDER: this_object(),
		USER: this_player()->query_real_name(),
		VERBOSE: verbose,
		DATA: arg,
	    ]), 1);
    }
    write("Trying to locate \"" + arg + "\": Please wait.\n");
    return 1;
}

status main(string arg) { return cmd_locate(arg); }


void udp_reply(mapping data) {
    object ob;

    if (!stringp(data[USER]) || !(ob = find_player(data[USER])))
	return;
    switch(data[VERBOSE]) {
	case V_SUCCESS:
	    if (!data[FOUND])
		return;
	    break;
	case V_NO_TIME_OUTS:
	    if (data[SYSTEM] == TIME_OUT)
		return;
	    break;
    }
    if (data[SYSTEM] == TIME_OUT)
	tell_object(ob, "locate@" + data[NAME] + ": Timed out.\n");
    else
	tell_object(ob, data[DATA]);
}

/*
 * wiz_cmd_inetd.c
 * Written by: Alcides@T'Mud
 * Based on examples by Nostradamus@Zebedee
 */
#pragma save_binary
#pragma strict_types

inherit "/cmd/std/command_driver";

#include <std.h>
#include <stdproperties.h>
#include <inetd.h>
/*
#include <debug.h>
*/

#define CHECK_SO_WIZ if (WIZ_CHECK < WIZ_NORMAL) return 0
#define SUCCESS_MESSAGE "Request transmitted.\n"

/* **************************************************************************
 * Return a list of which souls are to be loaded.
 * The souls are listed in order of command search.
 */
string *
get_soul_list()
{
	return ({ });
}

/* **************************************************************************
 * Return a proper name of the soul in order to get a nice printout.
 */
string
get_soul_id()
{
	return "intermud";
}

/* **************************************************************************
 * The list of verbs and functions. Please add new in alfabetical order.
 */
mapping
query_cmdlist()
{
    return ([
		"intermud":"intermud",
		"iquery":"iquery",
		"itell":"itell",
		"iman":"iman",
		"iwho":"iwho",
		"ifinger":"ifinger",
		"imuds":"imuds",
		"iping":"iping",
		"ilocate":"cmd_locate",
		"fls":"ftp_cmd",
		"fget":"ftp_cmd",
		"fput":"ftp_cmd",
           ]);
}

int
intermud(string msg)
{
    int is_emote, is_list;
    string channel;

    CHECK_SO_WIZ;

    notify_fail("Intermud failed.\n");
    channel = query_verb();
    if (msg[0] == ':')
	{
	   is_emote = 1;
	   msg = extract(msg, 1);
	}
    if (msg == "list")
	is_list = 1;
    if (member_array(channel, ({ "intermud" }) ) == -1)
	return 0;
    if (!msg)
	write("Usage: " + query_verb() + " <msg>\n");
    else {
    	mapping data, hosts;
	string *muds, name;
	int i;

        name = this_interactive()->query_real_name();

	data = ([
	    REQUEST: "channel",
	    SENDER: name,
	    "CHANNEL": channel,
	    DATA: msg
	]);
	if (is_emote)
	{
	    data["EMOTE"] = 1;
	    data["CMD"] = "emote";
	}
	if (is_list)
	    data["CMD"] = "list";
	for(i = sizeof(muds = m_indices(hosts = INETD->query("hosts")));
	i--; ) {
	    if (muds[i] != lower_case(LOCAL_NAME))
	    if (
	    (member_array("*", hosts[muds[i]][HOST_COMMANDS]) != -1 ||
	    member_array("channel", hosts[muds[i]][HOST_COMMANDS]) != -1))
		INETD->send_udp(muds[i], data);
	}
	data[NAME] = LOCAL_NAME;
	call_other(UDP_CMD_DIR + "channel", "udp_channel", data);
    }
    return 1;
}

int
itell(string str) {
    	string who, msg, retval, name, mudname, user;
	int i;

    	CHECK_SO_WIZ;

    	name = this_player()->query_real_name();

    	notify_fail("syntax: Itell player@mud message\n");
    	if (!str || sscanf(str, "%s %s", who, msg) != 2)
		return 0;
 	if (sscanf(who, "%s@%s", user, mudname) != 2)
		return 0;
	if (retval = INETD->send_udp(mudname, ([
		REQUEST: "tell",
		RECIPIENT: user,
		SENDER: name,
		DATA: msg
	    ]), 1))
	{
		notify_fail(retval);
		return 0;	
	}
	write(SUCCESS_MESSAGE);
	return 1;
}

int
ifinger(string arg) 
{
    	string who, mud, retval;

    	CHECK_SO_WIZ;

	notify_fail("syntax: ifinger player@mud\n");
    	if (arg && sscanf(arg, "%s@%s", who, mud) != 2) 
 		return 0;
	if (retval = INETD->send_udp(mud, ([
	    REQUEST: "finger",
	    SENDER: this_player()->query_real_name(),
	    DATA: who
	]), 1))
	{
		notify_fail(retval);
		return 0;
	}
	write(SUCCESS_MESSAGE);
	return 1;
}

int
iwho(string str) 
{
	string retval;

    	CHECK_SO_WIZ;

	notify_fail("syntax: iwho mudname\n");
	if (!str) return 0;
	if (retval = INETD->send_udp(str,
	    ([ REQUEST: "who", SENDER: this_player()->query_real_name() ]),
	 1))
	{
		notify_fail(retval);
		return 0;
	}
	write(SUCCESS_MESSAGE);
	return 1;
}

#define MUDS_BAR "\
-------------------------------------------------------------------------------"
int
imuds(string mud) {
    mapping hosts;
    int i;
    string m;
    mixed *muds;

    CHECK_SO_WIZ;

    muds = m_indices(hosts = INETD->query("hosts"));
/*
    if (mud)
    {
	mud = lower_case(mud);
	if (muds = hosts[mud])
	write(muds[HOST_NAME] + " - " + muds[HOST_IP]); 
	write(" Udp port: "+muds[HOST_UDP_PORT]+"\n");
	iquery(mud+" mud_port");
	iquery(mud+" version");
	iquery(mud+" inetd");
	iquery(mud+" email");
	iquery(mud+" commands");
	return 1;
    }
*/

    m ="Mudnames             Status   Last access";
    write(m + "\n" + MUDS_BAR + "\n");
    for(i = 0; i < sizeof(muds); i++) {
	write(sprintf("%-20s %s\n", hosts[muds[i]][HOST_NAME],
	(hosts[muds[i]][HOST_STATUS] ?
	    hosts[muds[i]][HOST_STATUS] > 0 ?
		"UP       " + ctime(hosts[muds[i]][HOST_STATUS])[4..15] :
		"DOWN     " + ctime(-hosts[muds[i]][HOST_STATUS])[4..15]
	    : "UNKNOWN  Never accessed.")));
    }
    return 1;
}

int
iman(string arg) 
{
    	string what, mud, retval;
 
    	CHECK_SO_WIZ;

	notify_fail("syntax: iman doc@mud\n");
    	if (arg && sscanf(arg, "%s@%s", what, mud) != 2) 
		return 0;
	if (retval = INETD->send_udp(mud, ([
	    REQUEST: "man",
	    SENDER: this_player()->query_real_name(),
	    DATA: what
	]), 1))
	{
		notify_fail(retval);
		return 0;
	}
	write(SUCCESS_MESSAGE);
	return 1;
}

int
iquery(string str)
{
	string where, what, retval;

	if (WIZ_CHECK < WIZ_ARCH && previous_object() != this_object())
		return 0;

	notify_fail("Iquery where with what ?\n"+
	"Options: commands, email, hosts, inetd, list, time, version\n");
	if (!str) return 0;
	if (!sscanf(str, "%s %s", where, what))
		return 0;
	if (retval = INETD->send_udp(where, ([
		REQUEST: "query",
		SENDER: this_player()->query_real_name(),
		DATA: what
		]), 1))
		{
			notify_fail(retval);
			return 0;
		}
	write(SUCCESS_MESSAGE);
	return 1;
}

int
iping(string mud)
{
	string retval;

	if (WIZ_CHECK < WIZ_ARCH) return 0;

	if (retval = INETD->send_udp(mud, ([
		REQUEST: "ping",
		SENDER: this_player()->query_real_name(),
		]), 1))
	{
		notify_fail(retval);
		return 0;
	}
	write(SUCCESS_MESSAGE);
	return 1;
}

#define FTP_ROOT_DIR	"/ftp/"
#define MAX_READ_LINES	1000
#define COMMAND		"cmd"
#define LOCAL		"local"
#define REMOTE		"remote"
#define USER		"user"

#define MUD		0
#define ARG1		1
#define ARG2		2

int 
valid_file(string file) {
    return stringp(file) && member_array("..", explode(file, "/")) == -1;
}

#define FOUND		"fnd"
#define USER		"user"
#define VERBOSE		"vbs"
#define V_SUCCESS	1
#define V_NO_TIME_OUTS	2

int 
cmd_locate(string arg) {
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
	if ( arr[i] != lower_case(LOCAL_NAME) &&
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

int 
ftp_cmd(string str) {
    string *arg;
    string tmp, command;
    mapping data;
    int i;

    seteuid(getuid());

    notify_fail("Syntax error.\n");
    if (!str)
	return 0;
    if (sizeof(arg = explode(str, " ")) < 1)
	return 0;
    data = ([ ]);
    switch(query_verb()) {
	case "fls":
	    data[REMOTE] = (sizeof(arg) > ARG1 ? arg[ARG1] : "");
	    command = "dir";
	    break;
	case "fget":
	    if (sizeof(arg) < 2)
		return 0;
	    command = "get";
	    data[REMOTE] = arg[ARG1];
	    if (sizeof(arg) > ARG2)
		data[LOCAL] = arg[ARG2];
	    else {
		data[LOCAL] = explode(arg[ARG1], "/");
		data[LOCAL] = data[LOCAL][sizeof(data[LOCAL])-1];
	    }
	    if (!valid_file(data[LOCAL])) {
		write("Illegal local filename.\n");
		return 1;
	    }
	    if (file_size(FTP_ROOT_DIR + data[LOCAL]) != -1) {
		write("Local file already exists.\n");
		return 1;
	    }
	    break;
	case "fput":
	    if (sizeof(arg) < 2)
		return 0;
	    command = "put";
	     data[LOCAL] = FTP_ROOT_DIR+arg[ARG1];
	    if (!data[LOCAL] || file_size(data[LOCAL]) <= 0) {
		write("Illegal local filename.\n");
		return 1;
	    }
	    if (sizeof(arg) > ARG2)
		data[REMOTE] = arg[ARG2];
	    else {
		data[REMOTE] = explode(arg[ARG1], "/");
		data[REMOTE] = data[REMOTE][sizeof(data[REMOTE])-1];
	    }
/*
	    for(i = 0, data[DATA] = "";
	    tmp = read_file(data[LOCAL], i, MAX_READ_LINES);
	    i += MAX_READ_LINES)
		data[DATA] += tmp;
*/
	    data[DATA] = read_file(data[LOCAL]);
	    break;
	default:
	    return 0;
    }
    data[REQUEST] = "ftp";
    data[SENDER] = this_object();
    data[USER] = this_player()->query_real_name();
    data[COMMAND] = command;
    if (tmp = INETD->send_udp(arg[MUD], data, 1))
    {
	notify_fail(tmp);
	return 0;
    }
    else
	write(SUCCESS_MESSAGE);
    return 1;
}

void
udp_reply(mapping data)
{
	switch(data[REQUEST])
	{
	  case "ftp":
		call_out("ftp_reply", 1, data);
		break;
	  case "locate":
	 	call_out("locate_reply", 1, data);
		break;
	  default:
	}
}

void 
ftp_reply(mapping data) {
    object ob;
    int rp, du;
    string tmp;
    mixed *d;

    seteuid(getuid());
    if (!data[USER])
	return;
    if (!data[COMMAND] || data[SYSTEM] == TIME_OUT) {
	if (ob = find_player(data[USER]))
	    tell_object(ob, (data[SYSTEM] == TIME_OUT ?
	    "ftp: " + data[COMMAND] + "@" + data[NAME] + " timed out.\n" :
	    data[DATA]));
	return;
    }
    switch(data[COMMAND]) {
	case "dir":
	    if (!data[DATA])
	    {   data[DATA] = "Ftp@"+data[NAME]+
			": No files in directory.\n";
		break;
	    }
	    rp = sizeof(d = explode(data[DATA], ":"));
	    tmp = "Ftp@"+data[NAME];
	    tmp += ": Directory listing.\n";
	    for(du = 0; du < rp; du+=2)
	    {
		d[du+1] = atoi(d[du+1]);
		if (d[du+1] == -2) 
		{	d[du] += "/";
			d[du+1] = "";
		}
		tmp += sprintf("%-20s %s\n", d[du], 
		 intp(d[du+1]) ? itoa(d[du+1]) : "");
	    }
	    data[DATA] = tmp;
	    break;
	case "get":
	    if (!data[LOCAL] || file_size(FTP_ROOT_DIR + data[LOCAL]) != -1 ||
	    !write_file(FTP_ROOT_DIR + data[LOCAL], data[DATA]))
		data[DATA] = "Ftp@"+data[NAME]+":Error writing local file: " +
		FTP_ROOT_DIR + data[LOCAL] + "\n";
	    else
		data[DATA] = "Ftp@"+data[NAME]+": Remote file retreived: " +
		FTP_ROOT_DIR + data[LOCAL] + "\n";
	    break;
	default:
	    return;
    }
    if (data[USER] && (ob = find_player(data[USER])))
	ob->catch_msg("\n" + data[DATA]);
}


void 
locate_reply(mapping data) {
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
	ob->catch_msg("locate@"+data[NAME]+": timed out.\n");
    else
    	ob->catch_msg(data[DATA]);
}


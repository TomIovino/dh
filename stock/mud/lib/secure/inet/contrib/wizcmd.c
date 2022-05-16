#include <mudlib.h>
#include <udp.h>
#define CHECK_SO_WIZ
#define SUCCESS_MESSAGE "Request transmitted.\n"

int
iquery(string str)
{
	string where, what, retval;


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


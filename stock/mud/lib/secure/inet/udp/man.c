#include <udp.h>

void udp_man(mapping data) {
    INETD->send_udp(data[NAME], ([
	REQUEST: REPLY,
	RECIPIENT: data[SENDER],
	ID: data[ID],
        DATA: "cmds/wiz/_man"->get_man_string(data[DATA])
    ]) );
}

#pragma verbose_errors
#pragma combine_strings
#include <mudlib.h>
#include <udp.h>

void udp_finger(mapping data) {
  log_file("bah", data[DATA]);
    INETD->send_udp(data[NAME], ([
	REQUEST: REPLY,
	RECIPIENT: data[SENDER],
	ID: data[ID],
        DATA:
          ("cmds/std/_finger"->get_finger_str(data[DATA],1))
    ]) );
}

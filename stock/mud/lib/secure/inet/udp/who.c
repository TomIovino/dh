#include <udp.h>
 
void udp_who(mapping data) {
    INETD->send_udp(data[NAME], ([
        REQUEST: REPLY,
        RECIPIENT: data[SENDER],
        ID: data[ID],
       DATA:  (string)"/cmds/std/_who"->get_who_string()
       ])
    );
}

#include <udp.h>
 
#define CMD_TELL "/cmds/std/_tell"
 
string sender_name, mud_name;
status is_intermud_tell() { return 1; }
string query_real_name() { return sender_name ? lower_case(sender_name) : 0; } 
string query_mud_name() { return mud_name ? lower_case(mud_name) : 0; }
void udp_tell(mapping data) {
    object ob;
    string reply;
    sender_name = capitalize(to_string(data[SENDER])) + "@" + data[NAME];
    mud_name = data[NAME];
    if (data[RECIPIENT] && (ob = find_player(data[RECIPIENT])) &&
       interactive(ob) && !ob->query_invis() &&
       !CMD_TELL->disallow_tell(ob,this_object())) {
             reply = (string) CMD_TELL->do_tell(ob,sender_name,
                                                (data[DATA]+"\n"), 0);
             INETD->send_udp(data[NAME], ([
                            REQUEST: REPLY,
                            RECIPIENT: data[SENDER],
                            ID: data[ID],
                            DATA: reply
             ]) );
    } else
        INETD->send_udp(data[NAME], ([
            REQUEST: REPLY,
            RECIPIENT: data[SENDER],
            ID: data[ID],
            DATA: sprintf("%-80=s", "Root@" + LOCAL_NAME +
            ": No such player: " + capitalize(data[RECIPIENT])) + "\n"
        ]) );
    sender_name = mud_name = 0;
}

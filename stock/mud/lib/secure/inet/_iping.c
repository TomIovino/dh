#include <udp.h>
#define SUCCESS (printf("inetd:  Request transmitted.\n"),1)
#define ERR(X)  (notify_fail(X),0)
 
status main(string mud) {
        string retval;
 
        notify_fail("iping: Syntax: iping <mudname>\n");
        if(!mud) return 0;
        if (retval = INETD->send_udp(mud, ([
                REQUEST: "ping",
                SENDER: this_player()->query_real_name(),
                ]), 1)) {
                        return ERR(retval);
                }
          return SUCCESS;
        }

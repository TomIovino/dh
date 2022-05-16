#include <udp.h>
#define SUCCESS (printf("inetd:  Request transmitted.\n"),1)
#define ERR(X)  (notify_fail(X),0)
 
status main(string arg) {
    string what, mud, retval;
    notify_fail("syntax: iman doc@mud\n");
    if(arg && sscanf(arg, "%s@%s", what, mud) != 2) 
        return 0;
    if (retval = INETD->send_udp(mud, ([
                   REQUEST: "man",
                   SENDER: (string)this_player()->query_real_name(),
                   DATA: what
                 ]), 1)) {
                return ERR(retval);
    }
    return SUCCESS;
}

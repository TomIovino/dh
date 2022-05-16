#include <udp.h>
#define SUCCESS (printf("inetd:  Request transmitted.\n"),1)
#define ERR(X)  (notify_fail(X),0)
 
status main(string str) {
    string where, what, retval;
 
    notify_fail("query:  Syntax:  query <mudname> <option> ?\n"+
         "Options: commands, email, hosts, inetd, list, time, version\n");
    if (!str) return 0;
    if (!sscanf(str, "%s %s", where, what)) return 0;
    if (retval = INETD->send_udp(where, ([
                REQUEST: "query",
                SENDER: this_player()->query_real_name(),
                DATA: what
                ]), 1))
                {
                        return ERR(retval);
                }
    return SUCCESS;
}

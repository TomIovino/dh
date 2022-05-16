#include <ansi.h>
#include <mudlib.h>
 
#define StartedBy ((string) SHUTD->query_started_by() )
#define Started   (  (int)  SHUTD->query_started()    )
 
 
status _timeleft(string arg) {
    string ret;
    if(!ret = SHUTD->query_timeleft_string())
    return printf("%sNo shutdown is scheduled\n",B_BLUE_F),1;
    return printf("%-=75s\n",
    sprintf("%sDragonheart will be rebooted on %s CST.\n\
You have %s of playing time remaining.\n\
Reboot was started by %s on %s CST.\n",
    B_BLUE_F,ctime(time()+(int)SHUTD->query_time_remaining()),
    ret,StartedBy,ctime(Started))),1;
}
 
status help() {
    return printf(
"Command:  timeleft\n"
"The 'timeleft' command shows the scheduled\n"
"shutdown time if one is in progress.\n\n"
          ),1;
}

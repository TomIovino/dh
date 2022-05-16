#include <mudlib.h>
#include <ansi.h>

#define times(X) ((string) SHUTD->time_to_str(X))
#define UpTime() ((int) SHUTD->uptime())
#define CREATE 815126400
#define DAYS(X) ((X) / 86400)
 
status main(string arg) {
    string ut;
    write(B_BLUE_F); 
    printf("Dragonheart first opened on %s and has been open\n",ctime(CREATE));
    printf("for players for %s.\n",times(time()-CREATE));
    printf("Dragonheart was last rebooted on %s.\n",ctime(UpTime()));
    printf("It has been running for %s.\n",times(time()-UpTime()));
    return 1;
}

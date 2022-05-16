// _usage.c -- Prints out data from the Usage_D
//    Geewhiz@DragonHeartIII per Ash's request

#include <ansi.h>

static mapping mdows = ([ "Sun":0, "Mon":1, "Tue":2, "Wed":3, "Thu":4, "Fri":5, "Sat":6 ]);
static mapping mmons = ([ "Jan": 0, "Feb": 1, "Mar": 2, "Apr": 3, "May": 4, "Jun": 5,
                          "Jul": 6, "Aug": 7, "Sep": 8, "Oct": 9, "Nov":10, "Dec":11 ]); 

void fetch_date(int dw, int mo, int dt, int hr, int mn, int sc, int yr) {
    string dow_s, mon_s;
    sscanf(ctime(),"%s %s %d %d:%d:%d %d",dow_s,mon_s,dt,hr,mn,sc,yr);
    dw = mdows[dow_s];
    mo = mmons[mon_s];
}


int main(string arg) {
    int hr, hour, trash;
    int i;
    i = 24;
    
    fetch_date(trash,trash,trash,&hr,trash,trash,trash);

    printf("      Usage Graph                                                Date / Hour\n"
           "   10   20   30   40   50   60   70   80   90   100\n");
hr++;      
    while(i--) {
        hour = hr++ % 24;
        write((string)"/secure/usage_d"->make_graph(hour) + "\n");
    }     
          
    printf("   10   20   30   40   50   60   70   80   90   100\n"
    B_GREEN_F " Green = Minimum Number //"
   B_YELLOW_F " Yellow = Number at Hours End //"
   B_RED_F " Red = Highest Use.\n" NORM);
    
    return 1;
    
}

int help(string x){
    main("please");
  return 1;
}

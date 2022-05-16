// #define TESTING
// #define LIMIT_TESTING
#define CRNAME ( this_player() ? \
                   capitalize((string)this_player()->query_real_name())\
                 :\
                   "__MASTER_OBJECT__" )
 
private int time;
private int wait;
private int started;
private string started_by;
 
void tell_all(string msg);
void the_end(int delay);
void shut_it_down();
 
int time_to_delay(int t) {
    int delay;
    switch(t) {
        case              0:  delay =     1; break;  //  1 sec
        case     1 ..    30:  delay =    10; break;  // 10 sec
        case    31 ..    60:  delay =    30; break;  // 30 sec
        case    61 ..   600:  delay =    60; break;  //  1 min
        case   601 ..  1800:  delay =   300; break;  //  5 min
        case  1801 ..  3600:  delay =   900; break;  // 15 min
        case  3601 .. 14400:  delay =  1800; break;  // 30 min
        case 14401 .. 28800:  delay =  7200; break;  //  2 hr
        case 28801 .. 43200:  delay = 14400; break;  //  4 hr
        case 43201 .. 86400:  delay = 21600; break;  // 12 hr 
        default            :  delay = 43200; break;  //  1 day
    }
    return delay;
}
 
string time_to_str(int t) {
    string ret;
    int n;
    if(t == 0) return 0;
    ret = "";
    if(n = t/86400) ret += sprintf("%c%d day%c",
                               (ret!="")?' ':  0 ,n,
                               (n==1) ? 0 : 's'), t %=  86400;
    if(n = t/3600) ret += sprintf("%c%d hour%c",
                               (ret!="")?' ':  0 ,n,
                               (n==1) ? 0 : 's'), t %=  3600;
    if(n = t/60) ret += sprintf("%c%d minute%c",
                               (ret!="")?' ':  0 ,n,
                               (n==1) ? 0 : 's'), t %=   60;
    if(t) ret += sprintf("%c%d second%c",
                        (ret != "")?' ':0,t,(t==1) ? 0 : 's');
    return ret;
}
 
status adj_time(int t) {
    int m,s,adj;
    if(t <= 10) return 0;
    m = t % 300;
    switch(m) {
        case 1..179: adj = negate(m); break;
        case 180..299: adj = 300 - m; break;
        default  : adj = 0; break;
    }
    if(!adj) return 0;
    return t += adj, 1;
}
 
 
int value(string s) {
    if(!s || !stringp(s)) return 0;
    switch(s[strlen(s)-1]) {
        case 'd': case 'D':            return 86400;
        case 'h': case 'H':            return  3600;
        case 'm': case 'M': default :  return    60;
    }
}        
 
int str_to_time(string s) {
    int h, m, n, total, argc;
    string *argv;
 
    argc = sizeof( argv = explode(s, " ") );
    total = 0;
    if(argc > 4) return -1;
    while(argc--) {
        if(!sscanf(argv[argc],"%d",n)) return -1;
        total += n * value(argv[argc]);
    }
    return total;
}
    
status _sdt(string arg) {
    int new_time;
    if(!arg || (new_time = str_to_time(arg)) == -1) {
       return notify_fail("sd: syntax:  sd <time>\n"),0;
    }
    if(adj_time(&new_time)) {
        printf("sdt: Time rounded to %s\n",time_to_str(new_time));
    }        
    time = new_time;
    while(time > 0) {
        printf("Shutdown in %s!\n",time_to_str(time));
        time -= time_to_delay(time);
    }
    return 1;
}

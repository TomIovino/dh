
//#define TESTING
//#define LIMIT_TESTING
//#define VERBOSE_TESTING
//#define INCLUDE_ASH

#include <ansi.h>
#define AR B_RED_F
#define CRNAME (this_player() ? to_string(this_player()->query_alt_name()) : "Ash")


string do_msg(string say){
    catch("/secure/channeld/channeld"->
    tell_channel("announce",say+".",":"));
    return say;
}
 
private int time;
private int wait;
private int started;
private int shut_time;
private string started_by;
private status UnStoppable;
private int Type = 0;

private int system_halt = 0;
 
 
void tell_all(string msg);
void the_end(int delay);
void shut_it_down();
 
int time_to_delay(int t) {
    int delay, tmp;

    switch(t) {
        case     0 ..    30:  delay =    10; break;  //          10 sec
        case    31 ..    60:  delay =    30; break;  //          30 sec
        case    61 ..   600:  delay =    60; break;  //        1 min
        case   601 ..  1800:  delay =   300; break;  //        5 min
        case  1801 ..  3600:  delay =   900; break;  //       15 min
        case  3601 .. 10800:  delay =  1800; break;  //       30 min
        case 10801 .. 21600:  delay = 10800; break;  //     3 hr
        case 21601 .. 43200:  delay = 21600; break;  //     6 hr
        case 43201 .. 86400:  delay = 43200; break;  //    12 hr 
        default            :  delay = 86400; break;  //  1 day
    }
    return (tmp = t % delay) ? tmp : delay;
}
 
string time_to_str(int t) {
    string ret;
    int n,x;
    if(t == 0) return "no time";
    ret = "";

    if(t > 600){ x = t/15; t = x*15; }        // hmm -ash
    if(t > 3600){ x = t/60; t = x*60; }       // hmm -ash
    if(t > 21600){ x = t/900; t =x*900; }     // hmm -ash

    if(n = t/31556926) ret += sprintf("%c%d year%c",
                               (ret!="")?' ':  0 ,n,
                               (n==1) ? 0 : 's'), t %=  31556926;
    if(n = t/2629744) ret += sprintf("%c%d month%c",
                                 (ret!="")?' ':  0 ,n,
                                 (n==1) ? 0 : 's'), t %=  2629744;
    if(n = t/604800) ret += sprintf("%c%d week%c",
                               (ret!="")?' ':  0 ,n,
                               (n==1) ? 0 : 's'), t %=  604800;
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
    
    return ret == "" ? "no time" : ret;
}
 
int value(string s) {
    if(!s || !stringp(s)) return 0;
    switch(s[strlen(s)-1]) {
        case 'w': case 'W':            return 604800;
        case 'd': case 'D':            return  86400;
        case 'h': case 'H':            return   3600;
        case 'm': case 'M': default :  return     60;
        case 's': case 'S':            return      1;
    }
}        

string sd_type() { return system_halt ? "Shutdown" : "Reboot"; }

int security() {
    return this_player() ? this_player()->query_security() : 99;
}
 
int str_to_time(string s) {
    int h, m, n, total, argc;
    string *argv;
 
    if(s == "now") return 0;
    if(s == "cancel") return -2;
    argc = sizeof( argv = explode(s, " ") );
    total = 0;
    if(argc > 5) return -1;
    while(argc--) {
        if(!sscanf(argv[argc],"%d",n)) return -1;
        total += n * value(argv[argc]);
    }
    return total ? total : -2;
}
 
string *query_cmd_aliases() { return ({ "shutdown", "halt" }); }
 
varargs    
status _sd(string arg, int t) {
    int new_time;
 
    if(query_verb()){
        if(query_verb() == "shutdown") {
            arg = "5m";
        }
        if(query_verb() == "halt") {
            if(security() < 100)
                return notify_fail("Hrm... nope, Armageddon doesn't think so.\n"),0;
            system_halt = 1;
            arg = "5m";
        } else {
            system_halt = 0;
        }
    }
    if(!arg && !t) 
        return notify_fail("sd: syntax:  sd <time>\n"
                           "             shutdown\n"
                           "             halt\n"),0;
    else if(arg && !t) new_time = str_to_time(arg);
    else if(!arg && t) new_time = t;
 
    if(new_time == -1)
        return notify_fail("sd: syntax:  sd <time>\n"
                           "             shutdown\n"
                           "             halt\n"),0;
 
    if(new_time < 0) {
      if(UnStoppable)
          return notify_fail("Cannot cancel this " + sd_type() + "!\n"),0;
      if(time <= 0) return remove_call_out("the_end"),1;
      write("Aborting shutdown"); 
      remove_call_out("the_end");
      write("...Done.\n");
      log_file("GAME_LOG", 
      sprintf("%s Armageddon stopped by %s (Time Left: %s)\n",ctime(),CRNAME,
              time_to_str(time)));
      do_msg(AR + sd_type() + " Canceled!"+NORM);
      shut_time = started = started_by = wait = time = Type = 0;
#ifdef VERBOSE_TESTING
      remove_call_out("ShutStat");
#endif
      return 1;
    } else if(time > 0) { 
      if(UnStoppable && new_time > time)
          return notify_fail("Cannot increase this " + sd_type() + "!\n"),0;
      write(sd_type() + " was already in progress...\n");
      write("Aborting previous " + sd_type());
      remove_call_out("the_end");
      write("...Done.\n");
      started = time();
      shut_time = started + new_time;
      started_by = CRNAME;
      log_file("GAME_LOG", 
      sprintf("%s Armageddon rescheduled by %s (Time Left: %s)\n",
              ctime(),CRNAME,
              time_to_str(new_time)));
      do_msg(AR+sd_type() + " rescheduled !"+NORM);      
    } else {
       remove_call_out("end_it");
      started = time();
      shut_time = started + new_time;
      started_by = CRNAME;
       log_file("GAME_LOG", 
       sprintf("%s Armageddon started by %s (Time: %s)\n",ctime(),CRNAME,
              time_to_str(new_time)));
    }
#ifdef VERBOSE_TESTING
      call_out("ShutStat",10);
#endif
    time = new_time;
    wait = 0;
    if(this_player() != this_object()
    && previous_object() != this_object()) Type = 0;
    return the_end(0),1;
}
 
void print_network_warning();
 
void the_end(int delay) {
    string time_str;
    time -= delay;
    if(time <= 0) return shut_it_down();
    time_str = time_to_str(time);
    do_msg(AR+ sd_type() +" in "+time_str+NORM);
    time = shut_time - time(); 
    call_out("the_end",wait = delay = time_to_delay(time),delay);
}
 
void shut_it_down() {
    log_file("GAME_LOG", ctime(time())+" Shutdown for Reboot.\n");
    log_file("BREAK", ctime(time())+"--\n");
    do_msg(AR+"System rebooting.. reconnect."+NORM);
#ifdef TESTING
    wait = 0;
    time = 0;
    do_msg(AR+"shutdown() called"+NORM);
#else
    if(system_halt)
        write_file("/.shutdown",ctime() + " : By Armageddon\n");
    shutdown();
#endif
}
 
int query_time_remaining() {
    int remaining;
    remaining = find_call_out("the_end");
    return (remaining == -1) ? 0 : ( time - (wait - remaining) );
}
 
string query_timeleft_string() {
    int tr;
    return (tr = query_time_remaining()) ? time_to_str(tr) : 0;
}
 
int query_started() { return started; }
 
#ifndef TESTING
string query_started_by() { return started_by; }
#else
string query_started_by() { return started_by + " *TESTING*"; }
#endif
 
#ifndef TESTING
void shutdown_warning() {
    int tr;
    if(time <= 0 || tr = query_time_remaining() > 21600) return;
    printf("\n[ %|74s ]\n[ %|74s ]\n\n",
    sprintf("WARNING: A reboot is scheduled for %s CST",ctime(time()+tr)),
    sprintf("%s remains before reboot",query_timeleft_string()));
}
#endif
 
void tell_all(string s) {
    closure shout_cl;
    shout_cl = lambda( ({'u,'s}), 
                       ({ #'?, ({ #'environment, 'u }),
                         ({ #'tell_object, 'u, ({ #'to_string, 's }) }) }) );
 
#ifdef TESTING
#ifdef LIMIT_TESTING
    s = sprintf("#LIMITED TEST#\n%s",s);
#else
    s = sprintf("#TEST#\n%s",s);
#endif
#endif
#ifdef LIMIT_TESTING
#ifdef INCLUDE_ASH
    if(find_player("ash"))
        tell_object(find_player("ash"),sprintf("%-=75s\n",s));
#endif
    if(find_player("geewhiz"))
        tell_object(find_player("geewhiz"),sprintf("%-=75s\n",s));
#else
    filter_array(users(), shout_cl, sprintf("%-=75s\n",s));
#endif
}    
 
void notify_destruct() {
    int t;
    t = time;
    _sd(to_string(0));
    if(t > 0) do_msg(AR+"Shutdown stopped"+NORM);
}
 
status help() {
return printf("\
Commands:  shutdown\n\
           sd <time>\n\
           halt\n\
\n\
The command 'shutdown' is actually a front-end to the 'sd' command with a\n\
5 minute shutdown delay time.\n\
\n\
'sd' is the main shutdown command.  It takes an argument <time> which is\n\
either a number indicating the number of minutes until shutdown or\n\
    now | <num_minutes> | [ <num_days>d ] [ <num_hours>h ] <num_minutes>m\n\
To shutdown immediatly       :  'sd now'\n\
            in 5 minutes     :  'sd 5'  OR  'sd 5m'\n\
            in 1 day 2 hours :  'sd 1d 2h'\n\
            at 2am           :  'sd 02:00'  ** not implemented **\n\
To cancel a shutdown         :  'sd 0' or 'sd -1' or 'sd cancel'\n\
\n\
"),1;
}
 
#ifdef VERBOSE_TESTING
void ShutStat() {
    find_player("geewhiz") ? command("timeleft",find_player("geewhiz")) :
                             0;
    if(time > 0) call_out("ShutStat",10);
}
#endif
 
 
int AutoShutDown;
int ForcedShutDown;
 
int uptime() { return to_int(__MASTER_OBJECT__->uptime()); }
 
void shut(int minutes) {
    set_this_player(this_object());
    Type = 3;
    UnStoppable = 1;
    _sd(0,minutes*60);
}
 
string query_real_name() {
     switch(Type) {
         case 1: return "Armageddon";
         case 2: return "the mud driver";
         case 3: return "the Master Object";
         default: return started_by;
     }
 
}
 
int query_security_level() { return 2000; }
 
void reset(int started) {
    int diff;
 
    AutoShutDown   = uptime() + str_to_time("10h");

    if(AutoShutDown < (time() + 60))
        AutoShutDown += time() + 300;

    ForcedShutDown = uptime() + str_to_time("14h");
    
    if(ForcedShutDown < (time() + 900))
        ForcedShutDown += time() + 900;

    printf("\nUptime() = %s\n",ctime(uptime()));
    printf("Auto     = %s\n",ctime(AutoShutDown));
    printf("Forced   = %s\n",ctime(ForcedShutDown));
    printf("Current  = %s\n\n",ctime());
 
   if(AutoShutDown
   && time <= 0
   && Type < 1
   && (diff = AutoShutDown - time()) < str_to_time("12h")) {
       set_this_player(this_object());
       Type = 1;
       _sd(0,diff);
       return;
   }
   if(ForcedShutDown 
   && time <= 0
   && Type < 2
   && (diff = ForcedShutDown - time()) < str_to_time("3h")) {
       set_this_player(this_object());
       Type = 2;
       UnStoppable = 1;
       _sd(0,diff);
       return;
   }
}
 
#if 0
 
#define DY 86400
#define HR 3600
#define MN 60
 
#define DAY    0
#define MONTH  1
#define DATE   2
#define HOUR   4
#define MINUTE 5
#define SECOND 6
#define YEAR   7
 
int abs_time(string ts) {
   mixed *p, *t;
printf("parts = %O\n",parts);
//  "Wed Jul 31 20:49:55 1996",
// ({ /* sizeof() == 5 */
//    "Wed",
//    "Jul",
//    "31",
//    "20:49:55",
//    "1996"
//  })
   p = explode(ctime()," ");
   p[TIME] = explode(parts[TIME],":");
   t = allocate(3);
   t = explode(ts,":");
}
   
#endif
 
 

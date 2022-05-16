/////////////////////////////////////////////////////////////////
// mudtime.c -- Functions for DragonHearts calendar
// Geewhiz@Crimsoensti // Copyright (c) 1998 Andrew T. Graham
/////////////////////////////////////////////////////////////////
// $Id: mudtime.c,v 1.1 1998/03/13 19:54:23 atgraham Exp atgraham $
/////////
// Change Log:
// $Log: mudtime.c,v $
// Revision 1.1  1998/03/13 19:54:23  atgraham
// o Added last_called variable (Ash did actually)
//
// Revision 1.0  1998/03/12 23:51:58  atgraham
// First Version
//
//
/////////

#include <mudlib.h>
#include <ansi.h>

#include <mudtime.h>

#define TIME_OF_CREATION        (815126400)

#define FIRST_MUD_YEAR          (1256)

#define AGE_OF_MUD              (time() - TIME_OF_CREATION)
//#define MUD_YEAR                (1200 * 14 * 7 * 52) //(60 * 60 * 24 * 7 * 15) //(7432520)

//#define MUD_YEAR                (60 * 60 * 24 * 7 * 52)
//#define MUD_YEAR                (50 * 60 * 24 * 7 * 52)
//#define MUD_YEAR                (40 * 60 * 24 * 7 * 52)
//#define MUD_YEAR                (32 * 60 * 24 * 7 * 52)
//#define MUD_YEAR                (30 * 60 * 24 * 7 * 52)
//#define MUD_YEAR                (28 * 60 * 24 * 7 * 52)
//#define MUD_YEAR                (24 * 60 * 24 * 7 * 52)
//#define MUD_YEAR                (20 * 60 * 24 * 7 * 52)
#define MUD_YEAR                (16 * 60 * 24 * 7 * 52)
//#define MUD_YEAR                (15 * 60 * 24 * 7 * 52)
//#define MUD_YEAR                (12 * 60 * 24 * 7 * 52)
//#define MUD_YEAR                (10 * 60 * 24 * 7 * 52)
//#define MUD_YEAR                (8 * 60 * 24 * 7 * 52)
//#define MUD_YEAR                (4 * 60 * 24 * 7 * 52)
//#define MUD_YEAR                (2 * 60 * 24 * 7 * 52)

//#define MUD_YEAR                (60 * 60 * 24 * 7 * 6)

#define MUD_WEEK                (MUD_YEAR / 52)
#define MUD_MONTH               (MUD_YEAR / 12)
#define MUD_DAY                 (MUD_WEEK / 7)
#define MUD_HOUR                (MUD_DAY / 24)
#define MUD_MINUTE              (MUD_HOUR / 60)
#define MUD_SECOND              (MUD_MINUTE / 60)
#define MUD_HALF_MINUTE         (MUD_MINUTE / 2)

#define MUD_QUARTER             (MUD_HOUR / 4)
   

varargs int convert_time(int t) {
    t = t - TIME_OF_CREATION;
//    t -= t % 60;
    return t;
}    

varargs int mud_time() {
    return AGE_OF_MUD; // - (AGE_OF_MUD % 60);
}

varargs int mud_time_this_year(int t) {
    return (t = (t ? t : mud_time()) % MUD_YEAR) < 0 ? -t : t;
}

varargs int mud_day(int t) {
    return (mud_time_this_year(t) / MUD_WEEK) % 7;
}

varargs int mud_month(int t) {
    return 1 + (mud_time_this_year(t) / MUD_MONTH) % 12;
}

varargs int mud_date(int t) {
    return 1 + (mud_time_this_year(t) / MUD_DAY) % (52 * 7 / 12);
}

varargs int mud_hour(int t) {
    return (mud_time_this_year(t) / MUD_HOUR) % 24;
}

varargs int mud_minute(int t) {
    return (mud_time_this_year(t) / MUD_MINUTE)  % 60;
}

varargs int mud_quarter(int t) {
    switch(mud_minute(t)) {
        case  0..14: return 0; break;
        case 15..29: return 1; break;
        case 30..44: return 2; break;
        case 45..59: return 3; break;
    }
    return 0;
}

varargs int mud_daynight(int t) {
    switch(mud_hour(t)) {
        case 5: return 0; break;
        case 6..16: return 1; break;
        case 17: return 2; break;
        case 18..23:
        case 0..4: return 3; break;
   }
   return 0;
}


varargs int mud_second(int t) {
    return 0; //mud_time_this_year(t) % 60;
}

varargs int mud_year(int t) {
    if(t == 1) {
        return 1;
    }
    return FIRST_MUD_YEAR + (t ? t : mud_time()) / MUD_YEAR;
}

string *_day_to_lday = ({ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" });
string *_day_to_day = ({ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" });

varargs string day(int d, int long) {
    return long ? _day_to_lday[d] : _day_to_day[d];
}
string *_month_to_month = ({   "Aih",  "Bei",  "Cey",  "Dih",  "Eaa",  "Fth",  "Geh",  "Hie",  "Iae",  "Jah", "Iaz", "Dez" });

string month(int m) {
    return _month_to_month[m-1];
}

varargs string mud_ctime(int t) {
    if(!t)  t = mud_time();
    if(t > (mud_time() + 50 * MUD_YEAR))
        t = convert_time(t);
    return sprintf("%3s %3s %02d %02d:%02d:%02d %4d",
                   day(mud_day(t)),
                   month(mud_month(t)),
                   mud_date(t),
                   mud_hour(t),
                   mud_minute(t),
                   mud_second(t),
                   mud_year(t));
}

string ord(int n) {
    string ret;
    ret = sprintf("%d",n);
//    1st, 2nd, 3rd, 4th, 5th, ... 10th, ... 20th, 21st, 22nd, 23rd, 24th... 30th
    if(strlen(ret) > 1) {
        if(ret[<2..<1] == "11"
        || ret[<2..<1] == "12"
        || ret[<2..<1] == "13")
            return ret + "th";
    }
    if(ret[<1] == '1')
        return ret + "st";
    if(ret[<1] == '2')
        return ret + "nd";
    if(ret[<1] == '3')
        return ret + "rd";
    return ret + "th";
}

string minute_str(int t) { 
    int min;
    min = mud_minute(t);
    if(!min)
        return "precisely";

//    return sprintf("the %s minute after",ord(min),min > 1 ? 's' : 0);
    return "the " + ord(min) + " minute after";
}

string hour_str(int t) {
    int hour;
    hour = mud_hour(t);
    if(hour == 0)
        return "midnight";
    if(hour == 12)
        return "noon";
    if(hour > 12)
        hour -= 12;
    return sprintf("the %s hour",ord(hour));
}
    
varargs string query_mud_time(int t) {
    if(!t)  t = mud_time();
    if(t > (mud_time() + 50 * MUD_YEAR))
        t = convert_time(t);
    return sprintf("%s %s, in %s's reign, on %s %s %s, of the year %d",
                minute_str(t),
                hour_str(t),
                (mud_hour(t) >= 5 && mud_hour(t) < 17) ? "Dawn" : "Dusk",                
                day(mud_day(t),1),
                month(mud_month(t)),
                ord(mud_date(t)),
                mud_year(t));
    return sprintf("%s %s, during %s's reign, on the %s day of %s, a %s in the %s year after the Creation", 
                   minute_str(t),
                   mud_hour(t) == 0 ? "midnight" : mud_hour(t) == 12 ? "noon" : "the " + ord(mud_hour(t)) + " hour",
                   (mud_hour(t) >= 5 && mud_hour(t) < 17) ? "Dawn" : "Dusk",
                   ord(mud_date(t)),
                   month(mud_month(t)),
                   day(mud_day(t),1),
                   ord(mud_year(t)));
}                   

int day;
int day_status;                      /* -1 night, 0 twilight, 1 day */
status light_adj;                    /* 1 sunrise, -1 sunset */ 
int dawn_time;                       /* last dawn, real time */
string apr;


void tell_everyone(string str) {
  object *user;
  int i;
  i = sizeof(user = users());
  while(i--) {
    if(!query_editing(user[i]) && environment(user[i]))
      tell_object(user[i]," " + str);
  }
}


/********************************************************************/
/* fn specs */

int query_dawn_time();

/* time & light */

static void cycle_light();                          /* day->night->day-> */
void change_time();                                 /* cycle */
//void mud_time();                                    /* 'mud' clock time */ 

status query_night();                        
status query_day();
status query_dawn();
status query_dusk();

int query_light_adjustment();                       /* light adjust +1,-1 */

status blueprintp(object ob) { return sizeof(explode(file_name(ob),"#")) == 1; }
status clonep(object ob) { return !blueprintp(ob); }


varargs
string timestr(int t, int sh) {
    string *parts;
    string ret;
    ret = (string) "/cmds/wiz/_sd"->time_to_str(t);
    if(!sh)
        return ret;
    t = sizeof(parts = explode(ret," "));
    while(t--)
        if(t % 2) {
            parts[t-1] = parts[t-1] + parts[t][0..0];
            parts[t] = "";
        }
    return implode(parts - ({ "" })," ");
}



/*********************************************************************/
/* reset - start up this object */

void reset(status arg) {
  int diff,x;
  if(arg) return;
  printf("Time_D Debug:\n"
         "\tMud Creation Date = %s (%d)\n"
         "\tMud Year = %s (%d)\n"
         "\tMud Week = %s (%d)\n"
         "\tMud Month = %s (%d)\n"
         "\tMud Day = %s (%d)\n"
         "\tMud Hour = %s (%d)\n"
         "\tMud Minute = %s (%d)\n"
         "\tMud Second = %s (%d)\n"
         "\tMud Half Minute = %s (%d)\n"
         "\tMud Qtr. Hour = %s (%d)\n",
         ctime(TIME_OF_CREATION),TIME_OF_CREATION,
         timestr(MUD_YEAR,1),MUD_YEAR,
         timestr(MUD_WEEK,1),MUD_WEEK,
         timestr(MUD_MONTH,1),MUD_MONTH,
         timestr(MUD_DAY,1),MUD_DAY,
         timestr(MUD_HOUR,1),MUD_HOUR,
         timestr(MUD_MINUTE,1),MUD_MINUTE,
         timestr(MUD_SECOND,1),MUD_SECOND,
         timestr(MUD_HALF_MINUTE,1),MUD_HALF_MINUTE,
         timestr(MUD_QUARTER,1),MUD_QUARTER);

  call_out("update_time",0);
//   call_out("test_time",0,0,5);
}

void test_time(int expected, int cnt) {
  int diff;
  printf("\n\ndeltaT = mud_time() - expected = %d\n",mud_time() - expected);
  diff = ((15 - mud_minute()%15) * MUD_MINUTE);
  if((MUD_QUARTER - diff) < 0)
    diff -= MUD_QUARTER;
  if(--cnt)
      call_out("test_time",diff,mud_time() + diff, cnt);
  printf("Qtr. Boundry is %s (%d) away.\n",timestr(diff,1),diff);
  printf("mud_ctime(mud_time() + boundry) = %s\n",mud_ctime(mud_time() + diff));
  printf("time() = %d  ctime() = %s\n",time(),ctime());
  printf("         mud_ctime() = %s\n",mud_ctime());
  printf("    query_mud_time() = %s\n\n",query_mud_time());
}  

mixed *time_change_notices = ({ ({  }), ({  }), ({  }) });

varargs
mixed *query_notices(int when) {
    if(when < 0) {
        return ({ query_notices(QUARTER_CHANGE),
                  query_notices(DAYNIGHT_CHANGE),
                  query_notices(HOUR_CHANGE) });
    }
    time_change_notices[when] -= ({ 0 });
    return time_change_notices[when];
}

int last_called;
object *already_called = ({ });

varargs void recieve_time_notices(object ob, int when) {
    if(!ob)
        ob = previous_object();
    when--;
    if(when < 0) {
        recieve_time_notices(ob,QUARTER_CHANGE);
        recieve_time_notices(ob,DAYNIGHT_CHANGE);
        recieve_time_notices(ob,HOUR_CHANGE);
        return;
    }        
    time_change_notices[when] -= ({ 0 });
    if(member_array(ob,time_change_notices[when]) == -1)
        time_change_notices[when] += ({ ob });
}

varargs void remove_time_notices(object ob, int when) {
    if(!ob)
        ob = previous_object();
    when--;
    if(when < 0) {
        remove_time_notices(ob,QUARTER_CHANGE);
        remove_time_notices(ob,DAYNIGHT_CHANGE);
        remove_time_notices(ob,HOUR_CHANGE);
        return;
    }        
    time_change_notices[when] -= ({ 0 });
    time_change_notices[when] -= ({ ob });
}

void call_time_change(object ob,int qtr, int hr,int mn,int day,int month,int date,int year,int when) {
    int ret;
    if(!ob)
        return;
    if(catch(ret = (int) ob->time_change(qtr,hr,mn,day,month,date,year))
    || !ret){
        time_change_notices[when] -= ({ ob });
    } 
    
}

void call_out_time_changes(object ob,int qtr, int hr,int mn,int day,int month,int date,int year,int when) {
    if(member_array(ob,already_called) != -1)
        return;
    call_out("call_time_change",0,ob,qtr,hr,mn,day,month,date,year,when);
    already_called += ({ ob });
}

void broadcast_time(int when, int data) {
    int q, h, m, dy, mn, dt, yr, i;
    q = mud_quarter();
    h = mud_hour();
    m = mud_minute();
    dy = mud_day();
    mn = mud_month();
    dt = mud_date();
    yr = mud_year();
    when--;
    if(when < 0)
        return;

    if(last_called != q) {
        last_called = q;
        already_called = ({ });
    }

    time_change_notices[when] -= ({ 0 });
    map_array(time_change_notices[when],#'call_out_time_changes,q,h,m,dy,mn,dt,yr,when);
}    
    
int last_hour = -1;
int last_quarter = -1;
int last_day_night = -1;
int last_day = -1;
int last_date = -1;
int last_month = -1;
int last_year = -1;


void update_time() {
    int hour, quarter, day_night, day, date, month, year, i, diff;

    diff = ((15 - mud_minute()%15) * MUD_MINUTE);
    if((MUD_QUARTER - diff) < 0)
        diff -= MUD_QUARTER;
    call_out("update_time",diff);

    quarter = mud_quarter();
    hour = mud_hour();
    year = mud_year();
    date = mud_date();
    day = mud_day();
    month = mud_month();
    day_night = mud_daynight();
    
    if(quarter != last_quarter) {
        last_quarter = quarter;
        broadcast_time(QUARTER_CHANGE,quarter);
    }

/*
    if(year != last_year) {
        last_year = year;
        broadcast_time(YEAR_CHANGE,year);
    }
*/

    if(last_hour != hour) {
        last_hour = hour;
        broadcast_time(HOUR_CHANGE,hour);
    }
       
    if(day_night != last_day_night) {
        last_day_night = day_night;
        switch(day_night) {   
            case 0:
                if(quarter == 0 && hour == 5)
                    tell_everyone(YELLOW_F "The sky turns to " YELLOW_F "Dawn." NORM "\n");
                day_status = 0;
                light_adj = 1;
                break;
            case 1: 
                if(quarter == 0 && hour == 6)
                    tell_everyone(YELLOW_F "Dawn brings light to the world." NORM "\n");
                day_status = light_adj = 1;
                break;
            case 2:
                if(quarter == 0 && hour == 17)
                    tell_everyone(B_BLUE_F "The sky turns to " B_BLUE_F "Dusk." NORM "\n");
                day_status = 0;
                light_adj = -1;
                break;
            case 3:
                if(quarter == 0 && hour == 18)
                    tell_everyone(B_BLUE_F "Dusk brings darkness to the world." NORM "\n");
                day_status = light_adj = -1;
                break;
        }
        broadcast_time(DAYNIGHT_CHANGE,day_night);
    }
    already_called = ({ });
}


  
status query_night() { return (day_status == -1); }
status query_day()   { return (day_status != -1); }
status query_dawn()  { return (!day_status && light_adj == 1);  }
status query_dusk()  { return (!day_status && light_adj == -1); }
int query_light_adjustment() { return light_adj; }


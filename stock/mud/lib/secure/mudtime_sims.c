#include "/include/fn/mudtime.h"

varargs int convert_time(int t) { return (int) MUDTIMED->convert_time(t); }
int mud_time() { return (int) MUDTIMED->mud_time(); }
varargs int mud_time_this_year(int t) { return (int) MUDTIMED->mud_time_this_year(t); }
varargs int mud_day(int t) { return (int) MUDTIMED->mud_day(t); }
varargs int mud_month(int t) { return (int) MUDTIMED->mud_month(t); }
varargs int mud_date(int t) { return (int) MUDTIMED->mud_date(t); }
varargs int mud_hour(int t) { return (int) MUDTIMED->mud_hour(t); }
varargs int mud_minute(int t) { return (int) MUDTIMED->mud_minute(t); }
varargs int mud_quarter(int t) { return (int) MUDTIMED->mud_quarter(t); }
varargs int mud_daynight(int t) { return (int) MUDTIMED->mud_daynight(t); }
varargs int mud_second(int t) { return (int) MUDTIMED->mud_second(t); }
varargs int mud_year(int t) { return (int) MUDTIMED->mud_year(t); }
varargs string day(int d, int long) { return (string) MUDTIMED->day(d,long); }
string month(int m) { return (string) MUDTIMED->month(m); }
varargs string mud_ctime(int t) { return (string) MUDTIMED->mud_ctime(t); }
string ord(int n) { return (string) MUDTIMED->ord(n); }
varargs string query_mud_time(int t) { return (string) MUDTIMED->query_mud_time(t); }
varargs void recieve_time_notices(object ob, int when) {
    set_this_object(previous_object());
    MUDTIMED->recieve_time_notices(ob,when);
}
varargs void remove_time_notices(object ob, int when) {
    set_this_object(previous_object());
    MUDTIMED->remove_time_notices(ob,when);
}
status query_night() { return (status) MUDTIMED->query_night(); }
status query_day()   { return (status) MUDTIMED->query_day(); }
status query_dawn()  { return (status) MUDTIMED->query_dawn(); }
status query_dusk()  { return (status) MUDTIMED->query_dusk(); }


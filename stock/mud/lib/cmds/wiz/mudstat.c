#include <mudlib.h>
 
int load_time = time();
 
int h_last = time();
int h_count = 0;
int h_low = 0;
int h_high = 0;
int h_sum = 0;
int h_time = 0;
 
int r_last = time();
int r_count = -1;
int r_low = 0;
int r_high = 0;
int r_sum = 0;
int r_time = 0;
 
int c_last = time();
int c_count = 0;
int c_low = 0;
int c_high = 0;
int c_sum = 0;
int c_time = 0;
 
int p_last = 0;
int p_count = 0;
int p_low = -1;
int p_high = -1;
int p_sum = 0;
int p_time = 0;
 
string mtime(int t) {
    if(t == load_time) return "(null)";
    return ctime(t);
}
 
string ts(int t) {
    string ret; 
    ret = (string)SHUTD->time_to_str(t);
    return ret ? ret : "(null)";
}
 
float avg(int s, int c) {
   if(c == 0) return to_float(0);
   return to_float(s) / to_float(c);
}
 
#define TELL(x,y,z,a) _tell(x,y,z,a)
 
#define CHANNEL(X) "/secure/channeld/channeld"->tell_channel("MudStat","Report",X)
 
void _tell(string x, int y, int z, int a) {
   object me;
   string s;
   s = sprintf("%s was %s (high = %s, low = %s)",x,ts(y),ts(z),ts(a));
   CHANNEL(s);
//   if(me = find_player("geewhiz"))
//       command(sprintf("wiz <mudstat> %s() took %s (high = %s)",x,ts(y),ts(z)),me);
//     tell_object(me,);
}
 
 
status main(string arg) {
    printf("Mud Status Report\t(data collected since %s)\n",ctime(load_time));
    printf("Player Count Statisitics:\n\tLast : %10d\n\tLow  : %10d\n\tHigh : %10d\n\tAvg. : %10.2f\n\n",
    p_time,p_low < 0 ? 0 : p_low,p_high < 0 ? 0 : p_high,avg(p_sum,p_count));
    printf("Reset() Statistics:\n\tCount: %10d\tLast : %s\n\tLast : %10d\t(%s)\n\tLow  : %10d\t(%s)\n\tHigh : %10d\t(%s)\n\tAvg. : %10.2f\n\n",
    r_count,mtime(r_last),r_time,ts(r_time),r_low,ts(r_low),r_high,ts(r_high),avg(r_sum,r_count));
//    printf("Clean_up() Information:\n\tCount: %10d\tLast : %s\n\tLast : %10d\t(%s)\n\tLow  : %10d\t(%s)\n\tHigh : %10d\t(%s)\n\tAvg. : %10.2f\n\n",
//    c_count,mtime(c_last),c_time,ts(c_time),c_low,ts(c_low),c_high,ts(c_high),avg(c_sum,c_count));
    printf("Heart_beat() Statistics: (%d objects with heart_beats)\n",sizeof(heart_beat_info()));
    printf("\tCount: %10d\tLast : %s\n\tLast : %10d\t(%s)\n\tLow  : %10d\t(%s)\n\tHigh : %10d\t(%s)\n\tAvg. : %10.2f\n",
    h_count,mtime(h_last),h_time,ts(h_time),h_low,ts(h_low),h_high,ts(h_high),avg(h_sum,h_count));
    return 1;
}
 
 
void heart_beat() {
    object *u;
    int i;
    string log;
    i = p_time = sizeof(u = users());
    if((p_low == -1) || (p_time < p_low) ) p_low = p_time;
    if((p_high == -1) || (p_time > p_high) ) p_high = p_time;
    p_sum += p_time;
    p_count++;
    if(! (p_count % 900) ) TELL("player_count", p_time, p_high, p_low);
 
    h_time = time() - h_last;
    h_last = time();
    if(!h_low  || h_time < h_low ) h_low = h_time;
    if(!h_high || h_time > h_high) h_high = h_time;
    h_sum += h_time;
    h_count++;
 
    if(h_time > 4) {
        TELL("heart_beat", h_time, h_high, h_low);
    }
}
 
void reset(int arg) {
    if(!arg) set_heart_beat(1);
    r_time = time() - r_last;
    r_last = time();
    if(!r_low  || r_time < r_low ) r_low = r_time;
    if(!r_high || r_time > r_high) r_high = r_time;
    r_sum += r_time;
    r_count++;
 
    if(r_time > 900) TELL("reset", r_time, r_high, r_low);
 
}
 
int set_p_high(int h){p_high = h; }
int clean_up(int ref) {
    c_time = time() - c_last;
    c_last = time();
    if(!c_low  || c_time < c_low ) c_low = c_time;
    if(!c_high || c_time > c_high) c_high = c_time;
    c_sum += c_time;
    c_count++;
    TELL("clean_up", c_time, c_high, c_low);
    return 1;
}
 
int player_count(int which) {
    switch(which) {
       case -1:  return p_low;
       case  1:  return p_high;
       default:
         return p_count;
    }
}        
string user_count_str() {
    return sprintf("User Counts (Low/Avg/Hi) %d/%d/%d",p_low,
                                                to_int(avg(p_sum,p_count)),
                                                p_high);
}
  string who_func_str(){
  return sprintf("Average Users Since Reboot %d",to_int(avg(p_sum,p_count)));
}

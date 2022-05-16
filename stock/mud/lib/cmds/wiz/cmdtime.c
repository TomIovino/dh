// Timer -- An Optimization Tool
// Reports utime, stime, utime+stime, and eval_costs for the given command
// Geewhiz@DoD (Formally Dreamtime) 950510G
// Later Imported To DragonMeet 950722G
//
 
int *_timer_on  = ({ 0, 0, 0 });
int *_timer_off = ({ 0, 0, 0 });
 
void timer_on() {
    int *ru;
    ru = rusage();
    _timer_on = ({ ru[0], ru[1], get_eval_cost() });
}
 
void timer_off(int cmd) {
    int *ru, ec;
    ec = get_eval_cost();
    ru = rusage();
    _timer_off = ({ ru[0], ru[1], ec, cmd });
}
 
void timer_results() {
    int ub, ue, ud;
    int sb, se, sd;
    int tb, te, td;
    int eb, ee, ed;
    
    ub = _timer_on[0]; ue = _timer_off[0]; ud = ue - ub;
    sb = _timer_on[1]; se = _timer_off[1]; sd = se - sb;
    eb = _timer_on[2]; ee = _timer_off[2]; ed = (eb - ee) - 7;
    tb = ub + sb; te = ue + se; td = te - tb;
    
    printf("\n%6s  %'='10s  %'='10s  %'='10s  %'='10s\n",
           "======","Utime","Stime","Total","Eval Cost");
    printf("%6s  %10d  %10d  %10d  %10d\n", "Start", ub, sb, tb, eb);
    printf("%6s  %10d  %10d  %10d  %10d\n", "Stop", ue, se, te, ee);
    printf("%6s  %10d  %10d  %10d  %10d\n", "Change", ud, sd, td, ed);
    printf("command() returned %10d\n\n",_timer_off[3]);
}
 
 
status _Time(string arg) {
    int cmd;
 
    if(!arg || !stringp(arg))
        return notify_fail("timer:  Syntax:  Time <cmd>\n"),0;
    timer_on();
    cmd = command(arg,this_player());
    timer_off(cmd);
    timer_results();
    return 1;
}
 
status main(string arg) { return _Time(arg); }

int time_change(int qtr, int hr,int mn,int day,int month,int date,int year) {
    printf("ctime() = %s\nmud_ctime() = %s\n",ctime(),mud_ctime());
    printf("Date = %02d/%02d/%02d (D %d)\n",month,date,year,day);
    printf("Time = %02d:%02d (Q %d)\n",hr,mn,qtr);
    return 1;
}

void reset(int arg) {
    if(arg) return;
    recieve_time_notices();
}


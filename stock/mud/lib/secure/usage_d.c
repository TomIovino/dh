// Usage_D -- A Usage Daemon v1.00
//   Geewhiz@DragonHeartIII per Ash's request
//

#include <ansi.h>

void update_usage_data();

string *TimeStamp  = ({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
int    *MinUsers   = ({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
int    *MaxUsers   = ({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
int    *NumUsers   = ({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
int    *NumPlayers = ({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
int    *PlayerMax  = ({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
int    *PlayerMin  = ({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
int    *NumWizards = ({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
int    *WizardsMin = ({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
int    *WizardsMax = ({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
int    *NumSamples = ({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });

void reset(int arg) {
    if(arg) return;
    restore_object("/ob_saves/mudlib/UsageD");
    update_usage_data();
}


int hour = -1;

static mapping mdows = ([ "Sun":0, "Mon":1, "Tue":2, "Wed":3, "Thu":4, "Fri":5, "Sat":6 ]);
static mapping mmons = ([ "Jan": 0, "Feb": 1, "Mar": 2, "Apr": 3, "May": 4, "Jun": 5,
                          "Jul": 6, "Aug": 7, "Sep": 8, "Oct": 9, "Nov":10, "Dec":11 ]); 

void fetch_date(int dw, int mo, int dt, int hr, int mn, int sc, int yr) {
    string dow_s, mon_s;
    sscanf(ctime(),"%s %s %d %d:%d:%d %d",dow_s,mon_s,dt,hr,mn,sc,yr);
    dw = mdows[dow_s];
    mo = mmons[mon_s];
}

int WizardCt = 0,
    PlayerCt = 0,
    AllUsrCt = 0;
    
void count_players() {
    object *pl, *wi;
    pl = users();
    wi = filter_array(pl, lambda( ({ 'u }), 
                                  ({ #'call_other, 'u, "query_security_level" }) ));
    pl = pl - wi;

    NumWizards[hour] = WizardCt = sizeof(wi);
    NumPlayers[hour] = PlayerCt = sizeof(pl);
    NumUsers[hour] = AllUsrCt = WizardCt + PlayerCt;
    NumSamples[hour]++;
    
    if(AllUsrCt < MinUsers[hour]) MinUsers[hour] = AllUsrCt;
    if(AllUsrCt > MaxUsers[hour]) MaxUsers[hour] = AllUsrCt;
}


string make_graph(int h) {
    string graph, bars;
    int len, l;
    
    if(h < 0 || h > 23) return "=======================  Started  ======================= " + ctime()[0..12]+":00";

    if(!TimeStamp[h]) return "..............................................................";
    
    graph = "";
    bars  = "###########################################################";

    if(MinUsers[h] > 0) {
        len += MinUsers[h]/2;
        graph += B_GREEN_F + bars[0..(MinUsers[h]-1)/2] + NORM;
    }

    if(NumUsers[h] > 0 && NumUsers[h] > len) {
        len += (l = (NumUsers[h] - MinUsers[h])/2);
        graph += B_YELLOW_F + bars[0..l-1] + NORM;
    }

    if(MaxUsers[h] > 0 && MaxUsers[h] > len) {
        len += (l = (MaxUsers[h] - NumUsers[h])/2);
        graph += B_RED_F + bars[0..l-1] + NORM;
    }

    bars  = "                                                            ";
    graph += bars[0..60-len+2] + B_BLUE_F + TimeStamp[h] + (NumSamples[h] < 30 ? B_WHITE_F "*" NORM : NORM);
    
    return graph;
}


void update_usage_data() {
    int trash, hr, mn, sc;

    while(remove_call_out("update_usage_data") != -1) /* remove */ ;
    call_out("update_usage_data",60);

    restore_object("/ob_saves/mudlib/UsageD");

    fetch_date(trash,trash,trash,&hr,&mn,&sc,trash);

    if(hr != hour) {
        make_graph(hour);
        hour = hr;
        NumSamples[hour] = 0;
        TimeStamp[hour] = ctime()[0..12]+":00";
        MinUsers[hour] = MaxUsers[hour] = sizeof(users());

    }
    count_players();
    save_object("/ob_saves/mudlib/UsageD");
}        

string get_graph() {
    return make_graph(hour);
}


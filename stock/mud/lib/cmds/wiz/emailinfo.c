#include <ansi.h>
 
#define SHUTD "/cmds/wiz/sd"
 
string short_time(string ts) {
    ts = subst(ts,"no time","<Not Idle>");
    ts = subst(ts," weeks","w");
    ts = subst(ts," days","d");
    ts = subst(ts," hours","h");
    ts = subst(ts," minutes","m");
    ts = subst(ts," seconds","s");
    return ts;
}
 
status main(string arg) {
    object *u;
    int i, j, color;
    mapping info;
    string *indices;
    i = sizeof( u = users() );
    if(i<30) color++;
    info = ([ ]);
    while(i--) {
        string site, name, email, idle;
        site = lower_case(query_ip_name(u[i]));
        name = to_string(u[i]->query_name());
        email = lower_case(to_string(u[i]->query_email()));
        idle = short_time(to_string(SHUTD->time_to_str(query_idle(u[i]))));
        if(!info[site]) info[site] = ({ });
        info[site] += ({ ({ name, email, idle }) });
    }
    i = sizeof( indices = m_indices(info) );
    while(i--) { 
        mixed *data;
        data = info[indices[i]];
        j = sizeof(data);
        if(!arg && (j == 1)) continue;
        if(color)
           printf("%s%s:%s\n",YELLOW_F,indices[i],NORM);
        else
           printf("%s:\n",indices[i]);
        while(j--) {
            if(color)
                printf("\t%s%-15s %s%-40s %-15s%s\n",B_RED_F,data[j][0],
                                               B_GREEN_F,data[j][1],data[j][2],
                                               NORM);
            else
                printf("\t%-15s %-40s %-15s\n",data[j][0],data[j][1],data[j][2]);
        }
    }
    return 1;
}

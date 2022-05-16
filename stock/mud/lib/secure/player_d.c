#include <player.cfg>
#include <valid.cfg>

string name; 
string alias_name; 
string alt_name; 
string long_desc;
string extra_info; 
mapping skills;
string last;
mapping stats; 
mapping maxstats;   
string race;
string *languages_known;
int al;
int level;
int pk;
static string al_title;
string alignment;
int experience;
int money;
int age;
int gender;
mapping RegistrationInfo;
string spouse;
string house;
string party;
string email;
int on;
int off;
string ip_number;
string called_from_ip;
string enter_room;
int security_level;
string pretitle;
string title;
int savings;
int total_exp;
int create;


#define PLAYER_OFFLINE 1
#define PLAYER_NOT_EXIST 0

status restore_player(string n) {
    int Restored;
    Restored = restore_object(SAVE_NO_BANISH + lower_case(n));
    if(!Restored && restore_object(SAVE_WIZARD+lower_case(n)))
        Restored = 1;
    if(!Restored && restore_object(SAVE_FIRST+lower_case(n)))
        Restored = 1;
    if(!Restored && restore_object(GET_SAVE_PLAYER(lower_case(n))))
        Restored = 1;
    return Restored;
}


mixed find_player_or_load(string n) {
    object who;
    if(!n) return 0;
    if((who = find_player(n)) && interactive(who))
        return who;
    if(name == n)
        return PLAYER_OFFLINE;
    return restore_player(n);
}


string query_race(string n) {
    mixed res;
    res = find_player_or_load(n);
    if(intp(res))
        switch(res) {
            case PLAYER_OFFLINE:
                return race;
                break;
            case PLAYER_NOT_EXIST:
            default:
                return 0;
                break;
        }
    if(objectp(res))
        return res->query_race();
    return 0;
}
string query_pretitle(string n) {
    mixed res;
    res = find_player_or_load(n);
    if(intp(res))
        switch(res) {
            case PLAYER_OFFLINE:
                return pretitle;
                break;
            case PLAYER_NOT_EXIST:
            default:
                return "";
                break;
        }
    if(objectp(res))
        return res->query_pretitle();
    return 0;
}

int query_level(string n) {
    mixed res;
    res = find_player_or_load(n);
    if(intp(res))
        switch(res) {
            case PLAYER_OFFLINE:
                return level;
                break;
            case PLAYER_NOT_EXIST:
            default:
                return 0;
                break;
        }
    if(objectp(res))
        return res->query_level();
    return 0;
}
int query_gender(string n) {
    mixed res;
    res = find_player_or_load(n);
    if(intp(res))
        switch(res) {
            case PLAYER_OFFLINE:
                return gender;
                break;
            case PLAYER_NOT_EXIST:
            default:
                return 0;
                break;
        }
    if(objectp(res))
        return res->query_gender();
    return 0;
}

string query_al_title(string n) {
    mixed res;
    res = find_player_or_load(n);
    if(intp(res))
        switch(res) {
            case PLAYER_OFFLINE:
                return alignment; //(al < 0) ? "evil" : ((al > 0) ? "good" : "neutral");
                break;
            case PLAYER_NOT_EXIST:
            default:
                return 0;
                break;
        }
    if(objectp(res))
        return res->query_alignment();
//        return res->query_al_title();
    return 0;
}

string query_ip(string n) {
    mixed res;
    res = find_player_or_load(n);
    if(intp(res))
        switch(res) {
            case PLAYER_OFFLINE:
                return ip_number;
                break;
            case PLAYER_NOT_EXIST:
            default:
                return 0;
                break;
        }
    if(objectp(res))
        return query_ip_number(res);
    return 0;
}



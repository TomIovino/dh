// Dismiss

#include <mudlib.h>
#include <ansi.h>
#include <bar.h>
#include <party.h>

string inactive_members(string party) {
    int i, n, j;
    string ret, *names;
    object who;
    ret = "";
    j = sizeof( names = (string *) PARTY_D->query_party_members(party) );
    if(j) for(i = n = 0; i < j; i++) {
        if(!(who = find_player(names[i]))
        || party != (string) who->query_party()) {
            ret += (n++ ? ", " : "") + capitalize(names[i]);
        }
    }
    return ret == "" ? "<no other members>" : ret;
}

varargs
string fix_party(string p, object who) {
    if(!p || !stringp(p) || p == "")
        return p;
    PARTY_D->fix_party_name(&p);
    if(who && objectp(who))
        who->set_party(p);
    return p;
}


status main(string str) {
    string party, name;
    int will_need_new_party_leader;
    int i, exp, rating, alt_rating, kills, total_levels, dob, dbg;
    float share;
    object *who;
    string tmp, pname, leader, creator;
    status flag;
    status f;
    party = (string) this_player()->query_party();
    fix_party(&party,this_player());

    if(!party || !stringp(party)) {
        this_player()->set_party(0);
        notify_fail("You do not belong to a party.\n");
        return 0;
    }

    name = (string) this_player()->query_real_name();
    if(!name || !stringp(name)) {
        notify_fail("You have no name.  *boggle*\n");
        return 0;
    }
    
    if(!PARTY_D->restore_party(party)
    || !PARTY_D->query_party_member(party,name)) {
        this_player()->set_party(0);
        notify_fail("You are no longer in a party or it no longer exists.\n");
        return 0;
    }
    tmp = "";
    who = (object *) PARTY_D->sort_members(party);
    dob = (int) PARTY_D->query_party_age(party);
    pname = (string) PARTY_D->query_party_display_name(party);
    exp = (int) PARTY_D->query_party_experience(party);
    rating = (int) PARTY_D->query_party_rating(party) / RATING_SCALE;
    alt_rating = (int) PARTY_D->query_party_alt_rating(party) / ALT_RATING_SCALE;
    kills = (int) PARTY_D->query_party_kills(party);
    leader = (string) PARTY_D->query_party_leader(party);
    creator = (string) PARTY_D->query_party_creator(party);
    total_levels = (int) PARTY_D->query_total_levels(who);
   
    printf(NORM PRINTBAR "\n");
    printf(MAGENTA_F "Party Name:     " B_GREEN_F "%s\n"   ,capitalize(pname));
    printf(MAGENTA_F "Party Creator:  " B_GREEN_F "%s\n"   ,capitalize(creator));
    printf(MAGENTA_F "Current Leader: " B_GREEN_F "%s\n"   ,capitalize(leader));
    printf(MAGENTA_F "Party Exp:      " B_GREEN_F "%9d\n"  ,exp);
    printf(MAGENTA_F "Party Rating:   " B_GREEN_F "%9d\n"  ,rating);
/*
    printf(MAGENTA_F "Alt. Rating:    " B_GREEN_F "%9d\n"  ,alt_rating);
*/
    printf(MAGENTA_F "Party Kills:    " B_GREEN_F "%9d\n"  ,kills);
    printf(MAGENTA_F "Party Created:  " B_GREEN_F "%s\n"   ,ctime(dob));
    printf(NORM PRINTBAR "\n");

    for(i=0; i<sizeof(who); i++) {
        if(PARTY_D->is_here(who[i])) {
            share = (float) PARTY_D->calc_share((int)who[i]->query_level(),total_levels);
        } else { 
            share = 0.0;
        }
    printf(B_RED_F "%-15s " YELLOW_F "(%|10s" YELLOW_F ")   " B_GREEN_F " %6.2f%%\n",
        capitalize((string)who[i]->query_name(1)),
        (string)who[i]->query_short_condition(),
        (float)share);
  }
  printf(NORM PRINTBAR NORM"\n");
  printf(MAGENTA_F "Other Members:  " B_GREEN_F "%-=40s\n",inactive_members(party));
  printf(NORM PRINTBAR NORM"\n");
  return 1;
}


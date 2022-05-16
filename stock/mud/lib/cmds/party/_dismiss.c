// Dismiss

#include <mudlib.h>
#include <ansi.h>
#include <bar.h>
#include <party.h>

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
    object who;
    string party, name, l_name;
    int leader, creator, need_new_leader;
 
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

    if(!str || !stringp(str)) {
        notify_fail("Dismiss who from the party?\n");
        return 0;
    }
    

    leader = name == (l_name = (string)PARTY_D->query_party_leader(party));
    creator = name == (string)PARTY_D->query_party_creator(party);
    need_new_leader = lower_case(str) == (string)PARTY_D->query_party_leader(party);

//printf("leader = %O\ncreator = %O\nneed_new_leader = %O\n",leader,creator,need_new_leader);

    if(!leader && !creator) {
        notify_fail("Only the Leader can dismiss other players.\n");
        return 0;
    }

    if(!PARTY_D->query_party_member(party,lower_case(str))) {
        notify_fail("That person is not in your party.\n");
        return 0;
    }

    who = find_player(lower_case(str));

//printf("who = %O\ncreator = %O\nparty = %O\nwho->query_party() = %O\n",who,creator,party,who->query_party());

    if(!creator
    && (!who
    || (who && (party != (string) who->query_party())))) {
        notify_fail("Only the Creator can dismiss party members who are not present.\n");
        return 0;
    }
    
    if(!PARTY_D->remove_party_member(party,lower_case(str))) {
        notify_fail("You cannot remove the party Creator.\n");
        return 0;
    }
       
    PARTY_D->party_shout(party,capitalize(str)+" has been dismissed from the party.",0);
    if(leader)
        this_player()->remove_followers(who);
    if(who && who->query_party() == party)
        who->set_party(0);
    if(need_new_leader)
        PARTY_D->assign_new_leader(party);
    
     
    return 1;
}


status help() {
    printf("Command:  dismiss <name>                               Party\n"
           "\n"
           "Allows the leader or creator of a party to dismiss a memember.\n"
           "Only the creator can dismiss players that are not logged on.\n"
           "\n"
           "See also:  recruit, join, leave\n");
    return 1;
}


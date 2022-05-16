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
    string party, name, leader;

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

    leader = (string)PARTY_D->query_party_leader(party);
    if(name == leader) {
        notify_fail("You can't follow yourself.  You are the leader.\n");
        return 0;
    }

    if(!who = find_player(leader)) {
        PARTY_D->assign_new_leader(party);
        leader = (string)PARTY_D->query_party_leader(party);
        if(!who = find_player(leader)) {
            notify_fail("Leader isn't around anywhere! (bug)\n");
            return 0;
        }
    }
    
    
    if(!present(who, environment(this_player()))) {
        notify_fail("The leader must be with you in the room.\n");
        return 0;
    }

    if(who->query_followers(this_player())) {
        printf("You were already following %s...\n",capitalize(leader));
        who->remove_followers(this_player());
        printf("You are no longer following %s.\n",capitalize(leader));
        return 1;
    }
    printf("You are now following %s.\n",capitalize(leader));
    tell_object(who,this_player()->query_name()+ " is now following you.\n");
    who->add_followers(this_player());
    return 1;
}


status help() {
    printf("Command:  follow                                        Party\n"
           "\n"
           "Toggles following the current party leader.  Type it once to\n"
           "start following, and again to stop.\n"
           "\n"
           "See also:  delegate\n");
    return 1;
}


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
    string party, name;
    int will_need_new_party_leader;

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

    if(name != (string)PARTY_D->query_party_leader(party)
    && name != (string)PARTY_D->query_party_creator(party)) {
        notify_fail("Only the leader can delegate a new leader.\n");
        return 0;
    }

    if(!str || !stringp(str)) {
        printf("Delegating default leader...\n");
        PARTY_D->assign_new_leader(party);
        return 1;
    }

    if(!PARTY_D->query_party_member(party,lower_case(str))) {
        notify_fail("That person is not in your party.\n");
        return 0;
    }

    if(!who = find_player(lower_case(str))) {
        notify_fail("That player is not online.\n");
        return 0;
    }

    if(!interactive(who)) {
        notify_fail("That person is linkdead!\n");
        return 0;
    }
    
    PARTY_D->set_party_leader(party,lower_case(str));
    PARTY_D->party_shout(party,sprintf("%s is now the leader!",capitalize(str)));
    return 1;
}

status help() {
    printf("Command:  delegate [ <name> ]                               Party\n"
           "\n"
           "The delegate command allows a party to shift leadership between\n"
           "members of the party.  This way each can take turns leading party\n"
           "members to different areas of the mud of which they may be more\n"
           "familiar.\n  Delegate by itself will choose a default leader,\n"
           "usually the creator, or the highest member on. If you specify a\n"
           "name then that player becomes the leader.  Only the current leader\n"
           "or the party creator can use this command.\n"
           "\n"
           "See also:  follow\n");
    return 1;
}


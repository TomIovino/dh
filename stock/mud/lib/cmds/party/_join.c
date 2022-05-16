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
    string party, name, old_party;
    int will_need_new_party_leader;

    old_party = (string) this_player()->query_party();
    fix_party(&old_party,this_player());

    name = (string) this_player()->query_real_name();

    if(!(party = str) || !stringp(party)) {
        notify_fail("Join what party?\n");
        return 0;
    }
    
    fix_party(&party);
   
    if(!PARTY_D->restore_party(party)) {
        notify_fail("That party does not exist.\n");
        return 0;
    }

    if(PARTY_D->query_party_member(party,name)
    && party == old_party) {
        notify_fail("You are already a member of that party!\n");
        return 0;
    }

    if(!PARTY_D->query_party_member(party,name)
    && !PARTY_D->is_invited(party,name)) {
        notify_fail("You are not invited to join that party.\n");
        return 0;
    }

    if(!PARTY_D->query_party_member(party,name)
    && sizeof((string *)PARTY_D->query_party_members(party)) >= 5) {
        notify_fail("That party already has 5 members.\n");
        return 0;
    }
    
    if(stringp(old_party)
    && PARTY_D->restore_party(old_party)
    && PARTY_D->query_party_member(old_party,name)
    && old_party != party) {
        printf("Forcing you to leave %O.\n",(string)PARTY_D->get_party_display_name(old_party));
        command("*leave party",this_player());
    }
    fix_party(&party);    
    this_player()->set_party(party);
    PARTY_D->add_party_member(party,name);
    if(PARTY_D->query_party_member(party,name)) {
        PARTY_D->party_shout(party,capitalize(name) + " has joined the party!", 0);
    }
    PARTY_D->remove_invite(party,name,1);
    return 1;
}

status help() {
    printf("Command:  join <party name>                               Party\n"
           "\n"
           "Allows a player to accept an invitation to join a party, or, if the\n"
           "player is already a member, rejoin a party.\n"
           "\n"
           "See also:  recruit, leave, dismiss\n");
    return 1;
}


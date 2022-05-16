// Disband

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

    if(!str || !stringp(str) || str != "party forever") {
        notify_fail("type: disband party forever\n");
        return 0;
    }
    
    if(name != (string)PARTY_D->query_party_creator(party)) {
      notify_fail("Only the party creator can disband the party.\n");
      return 0;
    }

    PARTY_D->party_shout(party,capitalize(name)+" has disbanded the party.", 0);
    PARTY_D->remove_party(party);
    return 1;
}

status help() {
    printf("Command:  disband party forever                               Party\n"
           "\n"
           "Allows the creator of a party to disband the party forever.\n"
           "All rankings etc will be LOST by doing this.\n"
           "\n"
           "See also:  form\n");
    return 1;
}


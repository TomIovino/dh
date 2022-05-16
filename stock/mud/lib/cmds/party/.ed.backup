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
        notify_fail("pchat what?\n");
        return 0;
    }

    if(str == "-h") {
        PARTY_D->party_chat_history(party);
        return 1;
    }
    if(str == "-c"
    && (name == (string) PARTY_D->query_party_leader(party)
     || name == (string) PARTY_D->query_party_creator(party))) {
        PARTY_D->clear_party_chat_history(party);
        printf("Party chat history cleared.\n");
        return 1;
    }
        

#define SOULD "/secure/soul/emote_d"

    if(str[0] == ';') {
        if(catch(str = (string)SOULD->get_feeling(str[1..],"tell"))) {
            notify_fail("error calling the emote_d\n");
            return 0;
        } else if(str) {
            str = sprintf(":%s", str);
        } else {
            return 0;
        }
    }
    
    PARTY_D->party_shout(party,str,capitalize(name));
    return 1;
}

status help() {
    printf("Command:  pchat [ -h | -c | <chat> | :<emotechat> | ;<feeling ]         Party\n"
           "\n"
           "The Party chat line is heard by all active members of your party.\n"
           "   pchat -h   will show the pchat history\n"
           "   pchat -c   will clear the history (for creators and leaders only)\n"
           "   The other ones are just like the regular channels\n"
           "       pchat blah   ->  [Party Name] Name : blah\n"
           "       pchat :blah  ->  [Party Name] Name blah\n"
           "       pchat ;smile ->  [Party Name] Name smiles happily.\n");
    return 1;
}


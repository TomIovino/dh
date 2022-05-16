// Leave
        
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
    string party;
    string name, l_name;
    object l_ob;
    int will_need_new_leader, forever;
    
    party = (string) this_player()->query_party();
    fix_party(&party,this_player());

    name = (string) this_player()->query_real_name();
    if(!party || !stringp(party)) {
        this_player()->set_party(0);
        notify_fail("You do not belong to a party.\n");
        return 0;
    }

    if(!PARTY_D->restore_party(party)) {
        this_player()->set_party(0);
        notify_fail("Your party no longer exists.  The Creator disbanded it forever.\n");
        return 0;
    }

    if(!PARTY_D->query_party_member(party,name)) {
        this_player()->set_party(0);
        return 0;
    }

    if(!str || !stringp(str) || str[0..4] != "party") {
        notify_fail("Leave party? or Leave party forever?\n");
        return 0;
    }

    if(str[<7..<1] == "forever") {
        if(name != (string)PARTY_D->query_party_creator(party))
            forever = 1;
        else {
            write("You are the party creator! You cannot leave forever.\n"
                  "If that is what you wish to do then 'disband party forever'\n");
            return 1;
        }
    }

    PARTY_D->check_leader(party);

    if(!l_name = (string)PARTY_D->query_party_leader(party)) {
        notify_fail("No party leader! (*boggle* This is a bug.)\n");
        return 0;
    }
    
    if(!l_ob = find_player(l_name)) {
        printf("No party leader object. (bug?) Continuing anyway...\n");
    }
    
    will_need_new_leader = 0;

    if(name == l_name) {
        will_need_new_leader = 1;
    }

    if(l_ob)
        l_ob->remove_followers(this_player());

    PARTY_D->party_shout(party,sprintf("%s has left the party%s.\n",
                           capitalize(name),(forever ? " forever" : "")));
    if(forever) PARTY_D->remove_party_member(party,name);
    this_player()->set_party(0);
    if(will_need_new_leader)
        PARTY_D->assign_new_leader(party);
        

    return 1;
}

int help() {
    printf("Command:  leave party [forever]                            Party\n"
           "\n"
           "Allows a member to temporarily leave the party.  They may rejoin\n"
           "by using the join command.  To leave the party permanently use the\n"
           "'forever' modifier.\n"
           "\n"
           "See also:  form, recruit, join, dismiss\n");
    return 1;
}


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
    string party, name, leader, v_name;
    object l_ob, victim, my_p_attack;
    
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
        notify_fail("You no longer belong to your party or it no longer exists.\n");
        return 0;
    }
    leader = (string) PARTY_D->query_party_leader(party);
    if(leader == name) {
        notify_fail("You cannot assist yourself.\n");
        return 0;
    }

    if(!l_ob = find_player(leader)) {
        notify_fail("Your leader could not be found. (bug)\n");
        return 0;
    }
    
    if(!victim = (object) l_ob->query_attack()) {
        notify_fail("Your leader is not attacking anything.\n");
        return 0;
    }
    
    if(victim == this_player()) {
        notify_fail("Why would you want to help kill yourself!?\n");
        return 0;
    }

    if(victim->is_player()) {
        notify_fail("\nYou may not assist player killing.  You must issue the kill command yourself.\n\n");
        return 0;
    }

    if(!environment(l_ob)) {
        notify_fail("Your leader does not have an environment. (bug)\n");
        return 0;
    }
    
    if(!environment(this_player())) {
        notify_fail("You do not have an environment. (bug)\n");
        return 0;
    }
    
    if(!present(victim,environment(this_player()))
    || !present(this_player(),environment(l_ob))) {
        notify_fail("You are not with the leader or not with the monster.\n");
        return 0;
    }

    v_name = (string) victim->query_cap_name();

    my_p_attack = (object) this_player()->query_primary_attack();

    if(my_p_attack == victim) {
        notify_fail("You are already attacking " + v_name + ".\n");
        return 0;
    }
    
    this_player()->add_secondary_attacker(victim);
    this_player()->set_primary_attack(victim);    
    if(my_p_attack && present(my_p_attack,environment(this_player()))) {
        printf("\nYou turn to attack %s.\n",v_name);
        say(sprintf("\n%s turns to attack %s.\n\n",(string)this_player()->query_name(),v_name));
        return 1;
    }
    printf("\nYou assist in killing %s.\n",v_name);
    say(sprintf("\n%s joins in the fight against %s.\n",(string)this_player()->query_name(),v_name));
    return 1;
}


status help() {
    printf("Command:  assist                                     Party\n"
           "\n"
           "Has you attack the same monster that the leader is attacking.\n"
           "This does NOT work when leader is attacking a player.\n"
           "\n"
           "See also:  delegate, follow\n");
    return 1;
}


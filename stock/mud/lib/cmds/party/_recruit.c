#include <mudlib.h>
#include <ansi.h>
#include <bar.h>
#include <party.h>

#define WIZARD_CHECK
#define ALIGNMENT_CHECK_ALLOW_NEUTRAL

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


    if(!str || !stringp(str)) {
        notify_fail("Recruit who into the party?\n");
        return 0;
    }

    if(name != (string)PARTY_D->query_party_leader(party)
    && name != (string)PARTY_D->query_party_creator(party)) {
        notify_fail("Only the leader can recruit people.\n");
        return 0;
    }
    
    str = lower_case(str);
    if(!who = find_player(str)) {
        notify_fail("That player is not online.\n");
        return 0;
    }
    if(who->query_invis()){
        notify_fail("That player is not online.\n");
        return 0;
    }

    if(find_player(str)->query_invis()) {
        notify_fail("That player is not online.\n");
        return 0;
    }
    if(PARTY_D->query_party_member(party,str)) {
        notify_fail("That player is already a member of your party!\n"
                    "Tell that player to type 'join "+PARTY_D->get_party_display_name(party)+"' to rejoin it.\n");
        return 0;
    }
    
#ifdef NO_WIZPLAYER
    if(this_player()->query_security_level() && !who->query_security_level()){
        notify_fail("You should know better than to recruit players.\n");
        return 0;
    }
#endif

#ifdef WIZARD_CHECK
    if(!this_player()->query_security_level() && who->query_security_level()){
        notify_fail("You can't recruit Creators.\n");
        return 0;
    }
#endif

#ifdef 0
 if((string)this_player()->query_alignment() != "neutral"){
   if((string)who->query_alignment() != "neutral"){
     if(((string) this_player()->query_alignment()) != 
        ((string) who->query_alignment())) {
         notify_fail("Thou canst not party with thine enemy!\n");
         return 0;
        }
     }
  }
#endif
#ifdef 0
    {
        int Ral, Pal, Npa;
        Ral = (int) this_player()->query_alignment();
        Pal = (int) who->query_alignment();
        if((Ral && Pal && Ral != Pal)) {
            notify_fail("Thou canst not party with thine enemy!\n");
            return 0;
        }
    }
#endif    

    if(party == (string) who->query_party()) {
        notify_fail(capitalize(str)+" is already a member of your party!\n");
        return 0;
    }

    if(PARTY_D->is_invited(party,str)) {
        notify_fail("That person is already invited to join the party!\n");
        return 0;
    }

    if(sizeof((string *)PARTY_D->query_party_members(party)) >= 5) {
        notify_fail("You may only have 5 members in your party.\n");
        return 0;
    }

    PARTY_D->invite(party,str);
    PARTY_D->party_shout(party,capitalize(str) + " has been invited to join the party!", 0);
    return 1;
}

status help() {
    printf("Command:  recruit <name>                               Party\n"
           "\n"
           "Allows the leader or creator of a party to invite a player into\n"
           "the party.  The player is only invited to join and isn't actually\n"
           "in the party until they use the join command.\n"
           "\n"
           "*** You may not party with people of opposite alignment ***\n"
           "\n"
           "See also:  join, leave, dismiss\n");
    return 1;
}


// Dismiss

#include <mudlib.h>
#include <ansi.h>
#include <bar.h>
#include <party.h>

#define NAME_D "/cmds/admin/_nameban"

#define FORCE_PARTY_NAME

varargs
string fix_party(string p, object who) {
    if(!p || !stringp(p) || p == "")
        return p;
//printf("p = %O\n",p);
    PARTY_D->fix_party_name(&p);
//printf("p = %O\n",p);
    if(who && objectp(who))
        who->set_party(p);
    return p;
}

status main(string str) {
    object who;
    string party, name, frag;
    int will_need_new_party_leader;

    name = (string) this_player()->query_real_name();

    if(!name || !stringp(name)) {
        notify_fail("You have no name.  *boggle* (bug)\n");
        return 0;
    }
    
    party = (string) this_player()->query_party();
    fix_party(&party,this_player());
       
    if(party && stringp(party)
    && PARTY_D->restore_party(party)
    && PARTY_D->query_party_member(party,name)) {
        notify_fail(sprintf("%-=75s\n",
           sprintf("You are still a member of the %s.  If you wish, you may 'leave' that"
                   " party and then form a new one.",
                   (string) PARTY_D->get_party_display_name(party))));
        return 0;                   
    }

    this_player()->set_party(0);

#ifdef FORCE_PARTY_NAME
    str = capitalize(name)+"'s Party";
#endif

    if(!str || !stringp(str)) {
        notify_fail("Form what party?\n");
        return 0;
    }
    
#ifndef FORCE_PARTY_NAME
    if(file_size("/log/HOUSE/"+party) > 0){
        notify_fail("That name is reserved.\n");
        return 0;
    }

    if(strlen(str) > 20) {
        notify_fail("Try something about... hmm... " + (strlen(str) - 20) + " characters shorter.\n");
        return 0;
    }

    if(NAME_D->name_is_banned(str,&frag)) {
        notify_fail(sprintf("That name contains the banned name fragment %O.\nChoose another name and try again.\n",frag));
        return 0;
    }
#endif

    
    party = str;

    if(PARTY_D->restore_party(str)) {
        write("There is already a party called '"+str+"'.\n");
        if(PARTY_D->query_party_member(str,name)) {
            write("   You are still a member... rejoining.\n");
            this_player()->set_party(fix_party(party));
            PARTY_D->party_shout(capitalize(name)+" rejoins the party!\n");
            return 1;
        }
#ifdef FORCE_PARTY_NAME
        notify_fail("That party name is in use. (You should never see this. Report to Geewhiz)\n");
#else
        notify_fail("That party name is in use.  Choose another.\n");
#endif
        return 0;
    }
    this_player()->set_party(fix_party(party));
//printf("party = %O\n",party);
    PARTY_D->add_party(party,name);
    party = fix_party(party);
    PARTY_D->set_party_leader(party,name);
#ifdef FORCE_PARTY_NAME
    write(BOLD+"You now lead "+PARTY_D->get_party_display_name(party)+"!\n"+NORM);
#else
    write(BOLD+"You now lead the '"+PARTY_D->get_party_display_name(party)+"' party!\n"+NORM);
#endif
    return 1;
}


status help() {
#ifdef FORCE_PARTY_NAME
    string name;
    string str;
    name = (string) this_player()->query_real_name();
    str = capitalize(name)+"'s Party";
#endif

    printf("Command:  form"            
#ifdef FORCE_PARTY_NAME
                             "                                            Party\n"
           "\n"
           "Form a party called \"" + str + "\". If you are already a member\n"
#else
                             " <party name>                               Party\n"
           "\n"
           "Form a party called <partyname>. If you are already a member\n"
#endif
           "it will rejoin you to the party.\n"
           "\n"
           "See also:  recruit, leave, dismiss\n");
    return 1;
}


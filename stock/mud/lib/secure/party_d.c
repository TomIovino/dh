////////////////////////////////////////
// party_d.c -- Party Daemon
// 
// 970623  Third party_d for DragonHeart, Second by Geewhiz
//
////////
// $Id: party_d.c,v 1.1 1998/05/28 08:33:03 atgraham Exp atgraham $
////////
// ChangeLog:
//   $Log: party_d.c,v $
//   Revision 1.1  1998/05/28 08:33:03  atgraham
//   Initial revision
//
//
////////

#include <mudlib.h>
#include <ansi.h>
#include <bar.h>
#include <party.h>
#include <rank_d.h>

#define DEBUG_WIZARD_NAME "geewhiz"
#define NO_PRINTD
#define NO_ASSERT
#include <debug_utils.h>

#include <bim.h>


mapping party_data = ([]);
static mapping party_cache = ([ ]);
static mapping invited = ([ ]);


string fix_party(string party) {
    if(!party || !stringp(party))
        raise_error("party_d:  fix_party():  Bad argument 1\n");
    party = subst(lower_case(party)," ","_");
    party = subst(party,".","_");
    party = subst(party,"*","_");
    party = subst(party,">","_");
    party = subst(party,"<","_");
    party = subst(party,"|","_");
    party = subst(party,"~","_");
    party = subst(party,"%","_");
    party = subst(party,"!","_");
    party = subst(party,"?","_");
    return party;
}    

string fix_party_name(string party) { return fix_party(&party); }

varargs string cap_words(string str, int blah) {
    if(!str || str == "")
        return str;
    return implode(map_array(explode(str," "),#'capitalize)," ");
}    

mapping new_party(string party, string creator) {
    mapping m;
    if(!party || !stringp(party))
        raise_error("party_d:  new_party():  Bad argument 1\n");
    if(!creator || !stringp(creator))
        raise_error("party_d:  new_party():  Bad argument 2\n");
    m = ([ ]);
    m[DIRTY] = 1;
    m[DISPNAME] = cap_words(party);
    m[NAME] = fix_party(party);
    m[CREATOR] = creator;
    m[MEMBERS] = ({ creator });
    m[LEADER] = creator;
    m[EXPERIENCE] = 0;
    m[DOB] = time();
    m[RATING] = 0;
    m[KILLS] = 0;
    m[VERSION] = CURRENT_VERSION;
//PRINTD(m);
    return m;
}

 
varargs
string get_save_file(string party,int extension) {
    int c;
ASSERT(party != 0 && stringp(party));
    fix_party(&party);
//    c = party[0];
//    if(c < 'a' || c > 'z') c = 'z';
//    return sprintf("/ob_saves/partyd/%c/%s%s",c,party,
      return sprintf("/ob_saves/partyd/%s%s",party,
          extension ? ".o" : "");
}

varargs
void save_party(int only_if_dirty) {
    if(!party_data || !mappingp(party_data) || !stringp(party_data[NAME]))
        return;
    if(only_if_dirty && !party_data[DIRTY]) return;
    party_data[DIRTY] = 0;
    save_object(get_save_file(party_data[NAME]));
}


void check_leader(string party);

static int IN_restore_party = 0;    // Allows restore_party() to be reentrant
static int IN_assign_new_leader = 0;
static int IN_check_leader = 0;

int *get_IN() { 
    return ({ IN_restore_party, IN_assign_new_leader, IN_check_leader });
}

void reset_IN() { 
   IN_restore_party =  IN_assign_new_leader = IN_check_leader = 0;
}
   

void CO_send_mail(string creator, string *all_members, string subject, string msg) {
    closure send_mail;
    int i;
    mapping mesg;
    mesg = ([ 
                SNDR : "PartyDaemon",
                RCPT : creator,
                CCOPY: implode(all_members - ({ creator }),", "),
                SUBJ : subject,
                MESSAGE : sprintf("%-=65s\n    -- The Party Daemon\n",msg),
                DATE : time(),
                COMPLETE: 1
           ]);
    send_mail = symbol_function("send_mail",find_object(BIM_DIR "bimd"));
    i = sizeof(all_members);
    while(i--) {
        funcall(send_mail,all_members[i],mesg,SF_QUIETLY);
    }
    printf("Finished mailing party members.\n");
}

void send_mail(string creator, string *all_members, string subject, string mesg) {
    printf("Sceduling an update mailing...\n");
    call_out("CO_send_mail",0,creator,all_members,subject,mesg);
}


void update_party_version() {
    int size;
    string *cut;
    if((size = sizeof(party_data[MEMBERS])) > 5) {
        party_data[MEMBERS] -= ({ party_data[CREATOR] });
        party_data[MEMBERS] = ({ party_data[CREATOR] }) + party_data[MEMBERS];
        cut = party_data[MEMBERS][5..];
        send_mail(party_data[CREATOR],party_data[MEMBERS],
             sprintf("Party %O Too Large",party_data[DISPNAME]),
             sprintf("This mail is being sent to notify you that the party %O "
             "was too large.  The following %d member(s) have been dismissed "
             "automatically:\n  %s\n\nThis leaves the party with the following "
             "members:\n %s\n",party_data[DISPNAME],size-5,
             implode(map_array(cut,#'capitalize),", "),
             implode(map_array(party_data[MEMBERS][0..4],#'capitalize),", ")));
        party_data[MEMBERS] = party_data[MEMBERS][0..4];
        party_data[DIRTY] = 1;
    }
    while(party_data[PARTYVER] != CURRENT_VERSION) {
        switch(party_data[PARTYVER]) {
            case 0:
            default:
                /* Do nothing, always check party group size now so
                 * just bump up version number
                 */
                break;
        }
        party_data[PARTYVER]++;
    }
}

status restore_party(string party) {
    int restored;
    mixed err;
    
    restored = 0;
  
   fix_party(&party);
ASSERT(party && stringp(party));
    if(IN_restore_party) {
        if(party_data[NAME] == party)
            return 1;
        return 0;
    }
    IN_restore_party = 1;


//PRINTD(party);
PRINTD(party_data);
//PRINTD(party_cache);

    if(party_data && mappingp(party_data)) {
        save_party(1);
        if(party_data[NAME] == party) {
            check_leader(party);
            IN_restore_party = 0;
            return 1;
        }
        if(stringp(party_data[NAME])) {
            party_cache[party_data[NAME]] = party_data;
            party_cache[party_data[NAME]][TOUCHED] = time();
        }
    }

    if(party_cache && mappingp(party_cache) && party_cache[party]) {
        party_data = party_cache[party];
        party_cache[party][TOUCHED] = time();
        check_leader(party);
        IN_restore_party = 0;
        return 1;
    }

    if(err = catch(restored = restore_object(get_save_file(party)))) {
        printf("*Could not restore party:\nErr = %-=70s",err);
    }
    
    if(restored) {
        update_party_version();
        check_leader(party);
    }
    
    IN_restore_party = 0;
    return restored;
}        

void set_party_data(string party, string key, mixed val) {
ASSERT(party && stringp(party));
ASSERT(key && stringp(key));
    fix_party(&party);
    if(!restore_party(party))
        raise_error(sprintf("party_d:  set_party_data():  No such party %O\n",party));
    party_data[DIRTY] = 1;
    party_data[key] = val;
//PRINTD(party_data);
//PRINTD(key);
//PRINTD(val);
}

string get_party_display_name(string party) {
ASSERT(party && stringp(party));
    if(!restore_party(party))
        return party;
    return party_data[DISPNAME];
}        

string query_party_display_name(string party) {
    return get_party_display_name(party);
}

varargs
void remove_invite(string party, string name, int quiet) {
    object who;
    string pname;
ASSERT(party && stringp(party));
ASSERT(name && stringp(name));
    if(!invited || !mappingp(invited))
        return;
    fix_party(&party);
    if(!invited[party])
        return;
    pname = get_party_display_name(party);
    if(member(invited[party],name) != -1) {
        if(!quiet && (who = find_player(name)))
            tell_object(who,"\nYour invitation to the \"" + pname + "\" party has expired.\n");
        invited[party] -= ({ name });
        if(!sizeof(invited[party])) {
             invited[party] = 0;
             m_delete(invited,party);
        }
    }
}

int is_invited(string party, string name) {
ASSERT(party && stringp(party));
ASSERT(name && stringp(name));
    if(!invited || !mappingp(invited))
        return 0;
    fix_party(&party);
    if(!invited[party])
        return 0;
    return member(invited[party],name) != -1;
}

void invite(string party, string name) {
    object who;
    string pname;
ASSERT(party && stringp(party));
ASSERT(name && stringp(name));
    if(!invited || !mappingp(invited))
        invited = ([ ]);
    fix_party(&party);
    if(!invited[party])
        invited[party] = ({ });
    if(member(invited[party],name) == -1) {
        invited[party] += ({ name });
        pname = get_party_display_name(party);
        if(who = find_player(name))
            tell_object(who,sprintf("\n%-=75s\n",
             "You have been invited to join the \"" + pname + "\".  If you want to "
             "join this party type 'join " + pname + "' within 60 seconds.\n"));
        call_out("remove_invite",60,party,name);
    }
}

        
            
void flush_party_mess() {
    string *keys;
    int i;
    call_out("flush_party_mess",300);

    reset_IN();

    if(party_cache && mappingp(party_cache)) {
        i = sizeof(keys = m_indices(party_cache));
        while(i--)         
            if(party_cache[keys[i]])
                if(party_cache[keys[i]][TOUCHED])
                    if((time() - party_cache[keys[i]][TOUCHED]) > 900)
                        m_delete(party_cache,keys[i]);
    }
}    
    


void reset(status arg) {
    if(arg) return;
    call_out("flush_party_mess",300);
    if(!party_data || !mappingp(party_data))
        party_data = ([  ]);
    if(!party_cache || !mappingp(party_cache))
        party_cache = ([ ]);
}



// FIXME:  don't restore party here.  We don't know if we actually need
//         it loaded or not.  maybe a file_size() check instead?
status party_exists(string party) {
    if(!party || !stringp(party))
        raise_error("party_d:  party_exists():  Bad argument 1\n");
    return file_size(get_save_file(party,1)) > 0;
}


mapping get(int i) {
    mapping m;
    switch(i) {
        case 0:
            m = party_data;
            break;
        case 1:
            m = party_cache;
            break;
        case 2:
            m = invited;
            break;
        default:
            raise_error("party_d:  get():  Bad argument 1\n");
            break;
    }
    return copy_mapping(m);
}
       

mapping get_party(string party) {
ASSERT(party && stringp(party));
    fix_party(&party);
    if(!restore_party(party))
        return 0;
    return party_data;
}


status add_party(string party, string creator) {
    string fixed;
ASSERT(party && stringp(party));
    fixed = fix_party(party);
    if(party_exists(fixed))
        return 0;
    restore_party(fixed);  // Clear out current party
    party_data = new_party(party, creator);
    save_party();
}

object *sort_members(string party);

varargs
void update_party_ranking(string party, int del);


status remove_party(string party) {
    object *who;
    int i;
ASSERT(party && stringp(party));
    if(!party_exists(party))
        return 0;
    if(i = sizeof(who = sort_members(party)))
        while(i--)
            if(who[i]) who[i]->set_party(0);
    update_party_ranking(party,1);
    party_cache = m_delete(party_cache,party_data[NAME]);
    party_data = 0;
    if(rm(get_save_file(party)+".o"))
        return 1;
    return -1;
}

string query_party_creator(string party) {
ASSERT(party && stringp(party));
    if(!restore_party(party))
        raise_error(sprintf("party_d:  query_party_creator():  No such party %O\n",party));
    return party_data[CREATOR];
}

status add_party_member(string party, string name);


status query_party_member(string party, string name) {
    int found;
ASSERT(party && stringp(party));
    if(!name || !stringp(name))
        raise_error("party_d:  query_party_member():  Bad argument 2\n");
    if(!restore_party(party))
        raise_error(sprintf("party_d:  query_party_member():  No such party %O\n",party));
//PRINTD(party_data[MEMBERS]);
    if(!party_data  
    || !mappingp(party_data))
        raise_error(sprintf("party_d:  query_party_member():  party_data = %O\n",party_data));
    if(!party_data[MEMBERS]
    || !pointerp(party_data[MEMBERS]))
        party_data[MEMBERS] = ({ });
    if(!found = (member((string *)party_data[MEMBERS],(string)name) != -1))
        if(name == query_party_creator(party)) {
            add_party_member(party,name);
            return 1;
        }
    return found;
}

status add_party_member(string party, string name) {
ASSERT(party && stringp(party));
    if(!name || !stringp(name))
        raise_error("party_d:  add_party_member():  Bad argument 2\n");
    if(!restore_party(party))
        raise_error(sprintf("party_d:  add_party_member():  No such party %O\n",party));
    if(query_party_member(party,name))
        return 0;
    party_data[MEMBERS] += ({ name });
    party_data[DIRTY] = 1;
    return 1;
}        

string query_party_leader(string party) {
ASSERT(party && stringp(party));
    if(!restore_party(party))
        raise_error(sprintf("party_d:  query_party_leader():  No such party %O\n",party));
PRINTD(party_data);
    return party_data[LEADER];
}

status remove_party_member(string party, string name) {
ASSERT(party && stringp(party));
    if(!name || !stringp(name))
        raise_error("party_d:  remove_party_member():  Bad argument 2\n");
    if(!restore_party(party))
        raise_error(sprintf("party_d:  remove_party_member():  No such party %O\n",party));
    if(!query_party_member(party,name)
    || name == query_party_creator(party))
        return 0;

   
    party_data[MEMBERS] -= ({ name });
    party_data[DIRTY] = 1;
    return 1;
}

status set_party_leader(string party, string name) {
    object who, *followers;
    int i;
ASSERT(party && stringp(party));
    if(!name || !stringp(name))
        raise_error("party_d:  remove_party_member():  Bad argument 2\n");
    if(!restore_party(party))
        raise_error(sprintf("party_d:  remove_party_member():  No such party %O\n",party));
    if(!query_party_member(party,name))
        add_party_member(party, name);

    if(party_data[LEADER])
        if(who = find_player(party_data[LEADER]))
            if(followers = (object *) who->query_all_followers())
                if(i = sizeof(followers))
                    while(i--)
                        if(followers[i]) {
                            who->remove_followers(followers[i]);
                            tell_object(followers[i],"\nYou no longer follow " +capitalize(party_data[LEADER]) + ".\n");
                        }

PRINTD(name);
    party_data[LEADER] = name;
PRINTD(party_data[LEADER]);
    party_data[DIRTY] = 1;
ASSERT(party_data[LEADER] == name);
PRINTD(party_data);

    return 1;
}


string *query_party_members(string party) {
ASSERT(party && stringp(party));
    if(!restore_party(party))
        raise_error(sprintf("party_d:  query_party_members():  No such party %O\n",party));
    if(!party_data[MEMBERS] || !sizeof(party_data[MEMBERS])) {
        if(query_party_creator(party))
            add_party_member(party,query_party_creator(party));
        else
            return 0;
    }
    return party_data[MEMBERS];
}

int query_party_experience(string party) {
ASSERT(party && stringp(party));
    if(!restore_party(party))
        raise_error(sprintf("party_d:  query_party_experience():  No such party %O\n",party));
    return party_data[EXPERIENCE];
}

int query_party_rating(string party) {
ASSERT(party && stringp(party));
    if(!restore_party(party))
        raise_error(sprintf("party_d:  query_party_rating():  No such party %O\n",party));
         if(party_data[RATING] > 2000000000)
         return 2000000000;
    return party_data[RATING];
}

int query_party_kills(string party) {
ASSERT(party && stringp(party));
    if(!restore_party(party))
        raise_error(sprintf("party_d:  query_party_kills():  No such party %O\n",party));
    if(party_data[KILLS] > 2000000000)
         return 2000000000;
    return party_data[KILLS];
}

int query_party_alt_rating(string party) {
ASSERT(party && stringp(party));
    if(!restore_party(party))
        raise_error(sprintf("party_d:  query_party_alt_rating():  No such party %O\n",party));
    return party_data[ALTRATING];
}

int query_party_age(string party) {
ASSERT(party && stringp(party));
    if(!restore_party(party))
       raise_error(sprintf("party_d:  query_party_age():  No such party %O\n",party));
 
   if(party_data[DOB]+43200 < time()){
    // essentially restart party data daily. (Ash)
    set_party_data(party,DOB,time());
    set_party_data(party,EXPERIENCE,0);
    set_party_data(party,RATING,0);
    set_party_data(party,KILLS,0);
    }      
    save_party();
    return party_data[DOB];
}

void party_chat_history(string party) {
ASSERT(party && stringp(party));
    if(!restore_party(party))
        raise_error(sprintf("party_d:  party_chat_history():  No such party %O\n",party));
    if(!party_data[CHATHIST])
        printf("No history for %s channel.\n", get_party_display_name(party));
    else
        printf("History for %s channel.\n%@s", get_party_display_name(party), 
            party_data[CHATHIST]);
}
void clear_party_chat_history(string party) {
ASSERT(party && stringp(party));
    if(!restore_party(party))
        raise_error(sprintf("party_d:  clear_party_chat_history():  No such party %O\n",party));
    party_data[CHATHIST] = 0;
    party_data[DIRTY] = 1;
}


mapping query_party_data() {
    return copy_mapping(party_data);
}

string fmt_names(string *names) {
//    string ret;
//    int i, s;
//    ret = "";
    return implode(map_array(names,#'capitalize),", ");
//    for(i = 0, s = sizeof(names); i < s; i++)
//        ret += (i ? ", " : "") + capitalize(names[i]);
//    return ret;
}


varargs
void update_party_ranking(string party, int del) {
  query_party_age(party);
ASSERT(party && stringp(party));
    if(!restore_party(party))
        raise_error(sprintf("party_d:  update_party_rankig():  No such party %O",party));
// Rankings Board Stub /////////////////////////////////////////////
catch(RANK_D->update_ranking(!del ? R_PARTY : -R_PARTY,party_data[DISPNAME],
                                       party_data[RATING],
                                       party_data[KILLS],
                                       capitalize(party_data[CREATOR])));
////////////////////////////////////////////////////////////////////
}

static int *n = ({ 0,  3,  1, 11,  5,  8, 11 }); 
//         --  --  --  --  --  --  --
static int *d = ({ 1,  4,  1, 10,  4,  5,  5 });

private static int add_rating(string party, int xp, int num) {
    int r_tmp;
ASSERT(party && stringp(party));
    if(!xp || !intp(xp))
        raise_error("party_d:  add_rating():  Bad argument 2\n");
    if(!intp(num))
        raise_error("party_d:  add_rating():  Bad argument 3\n");
    if(!restore_party(party))
        raise_error(sprintf("party_d:  add_rating():  No such party %O",party));

    if(num >= sizeof(n)) num = sizeof(n) - 1;
    r_tmp = xp * n[num] / d[num];

    party_data[RATING] += r_tmp;
    party_data[DIRTY] = 1;

    update_party_ranking(party);

    return r_tmp;
}




private static int round(float f) {
    return to_int(f + 0.5);
}


static by_level(object a, object b) {
    return a && b && ((int)a->query_level() > (int)b->query_level());
}

static by_interactive(object a, object b) {
    return a && b && interactive(a);
}

status is_here(object who) {
//    return interactive(who)               // Not interactive? not here.
//        && previous_object()
//        && this_player()
       return 1 && who && this_player()
//        && environment(previous_object()) // Not in room with killer? not here.
        && environment(this_player()) // Not in room with killer? not here.
//        && present(who,environment(previous_object()));
        && present(who,environment(this_player()));
}


status is_here_not_elm(object who) {
    return is_here(who) && !who->is_elemental();
}

object *sort_members(string party) {
    int i, j, dbg;
    object *who, tmp;
    string tp_party;
    who = ({ });
    
    if(!party || !restore_party(party))
        return who;

    if(this_player())
        tp_party = (string) this_player()->query_party();
    
    if(!j = sizeof(party_data[MEMBERS]))
        return who;
    for(i = 0; i < j; i++) {
        if(tmp = find_player(party_data[MEMBERS][i])) {
            if(party == (string) tmp->query_party()) {
/*
                if(tp_party && tp_party != party && tmp->query_invis())
*/
      if(tp_party != party && tmp->queery_invis())
                    continue;
                who += ({ tmp, });
                if(tmp = (object) tmp->query_attrib("elemental")) 
                    who += ({ tmp });
            }
        }
    }
    return sort_array(sort_array(who, #'by_level), #'by_interactive);
}

string strip_trailing_newlines(string mesg) {
    while(mesg[<1] == '\n')
        mesg = mesg[0..<2];
    return mesg;
}

varargs void party_shout(string party, string message, string name, int no_hist) {
    object *who;
    int i, room, emote;
    string pname, mesg;
ASSERT(party && stringp(party));
    if(!message || !stringp(message)) 
        raise_error("party_d:  party_shout():  Bad argument 2\n");

    strip_trailing_newlines(&message);

    if(!name || !stringp(name))
        name = "Update";

    pname = get_party_display_name(party);

    if (emote = (message[0] == ':'))
        message = message[1..];

    mesg = sprintf(emote ? "%s %s" : "%s : %s",
        name, message);

    mesg = sprintf("[%s] %-=*s",
            pname, 75 - strlen(pname), mesg);
    mesg = sprintf(BLUE_B B_CYAN_F "%s" NORM "\n", mesg);
   
    if(who = sort_members(party))
        map_array(who,#'tell_object,mesg);

    mesg = sprintf(emote ? "%s %s" : "%s : %s",
        name, message);

#define CHANNELD "/secure/channeld/channeld"

    CHANNELD->tell_channel("party",sprintf("[%s]",pname),":"+mesg, no_hist);

    mesg = sprintf("[%s] %-=*s\n",
        ctime(time())[11..15], 70, mesg);

    if(!no_hist) {
        if (!party_data[CHATHIST])
            party_data[CHATHIST] = ({ mesg });
        else
            party_data[CHATHIST] += ({ mesg });
        if (sizeof(party_data[CHATHIST]) > 20)
            party_data[CHATHIST] = party_data[CHATHIST][1..];
        party_data[DIRTY] = 1;
    }


} 


int party_leader_valid(string party) {
    object who;
    if(!party
    || !stringp(party)
    || !restore_party(party))
        return 0;

    if(party_data[LEADER]
    && (who = find_player(party_data[LEADER]))
    && interactive(who)
    && party == (string) who->query_party())
        return 1;
    return 0;
}


void assign_new_leader(string party) {
    object *all, who;
    string name, leader;
    int i, j;
//    if(IN_assign_new_leader)
//        return;

    IN_assign_new_leader = 1;

    if(!party || !stringp(party)) {
        IN_assign_new_leader = 0;
        raise_error("party_d:  assign_new_leader():  Bad argument 1\n");
    }

    fix_party(&party);

    if(!restore_party(party)) {
        IN_assign_new_leader = 0;
        raise_error(sprintf("party_d:  assign_new_leader():  No such party %O",party));
    }

PRINTD(party_leader_valid(party));

    if(party_leader_valid(party)) {
        IN_assign_new_leader = 0;
        return;
    }
 
PRINTD(sort_members(party));
    if(j = sizeof(all = sort_members(party))) {
PRINTD(j);
PRINTD(all);
PRINTD(!party_leader_valid(party));
        for(i = 0; (!party_leader_valid(party)) && (i < j); i++) {
            if((leader = (string) all[i]->query_real_name())
            && stringp(leader)) {
PRINTD(leader);
                set_party_leader(party,leader);
PRINTD(party_data);
PRINTD(query_party_leader(party));
PRINTD(party_leader_valid(party));
            }
        }
        
    }
    if(leader
    && stringp(leader)) {
        party_shout(party,sprintf("%s is now the leader!",capitalize(leader)));
    }
    IN_assign_new_leader = 0;
}

void check_leader(string party) {
    object who;
    if(IN_check_leader) return;
    IN_check_leader = 1;
    if(!party
    || !stringp(party)
    || !restore_party(party)) {
        IN_check_leader = 0;
        return;
    }
    if(party_leader_valid(party)) {
        IN_check_leader = 0;
        return;
    }
    assign_new_leader(party);
    IN_check_leader = 0;
}

int query_total_levels(object *who) {
  int i, total_levels;
  total_levels = 0;
  for(i=0; i<sizeof(who); i++) 
      if(is_here(who[i]))
          total_levels += (int)who[i]->query_level();
  return total_levels;
}

float calc_share(int level, int total_levels) {
  return (float) level / (float) total_levels * (float) 100.00;
}

void share_exp(int xp) {
    object *who;
    int i, j, num, r_incr, alt_r_incr, m_lev, cheating, bonus;
    float exp, share;
    string party, name, ret;
    int total_levels;

    if(!xp) return;

    cheating = caller_stack_depth() != 2;
cheating = 0;

    party = (string) this_player()->query_party();
    if(!party || !stringp(party) || !restore_party(party)) {
        this_player()->set_party(0);
        this_player()->add_exp(xp);
        return;
    }

    name = (string) this_player()->query_name(1);
    if(!name || !stringp(name) || (!query_party_member(party,name)
                                 && !this_player()->is_elemental())) {
        this_player()->add_exp(xp);
        return;
    }
  
    total_levels = query_total_levels(who = sort_members(party));

    if(xp > (total_levels * MAX_EXP_PER_LEVEL)) 
        xp = total_levels * MAX_EXP_PER_LEVEL;
  
    ret = "";
    num = 0;

    if(i = sizeof(who)) {
#if 1
        if((j = sizeof(filter_array(who,"is_here_not_elm"))) > 1) {
            bonus = (xp / 10) * j;
        } else
#endif
            bonus = 0;
        xp += bonus;
        while(i--) {
            if(is_here(who[i]) && !cheating) {
                num++;
                share = calc_share((int)who[i]->query_level(),total_levels);
            } else {
                share = 0.0;
            }
            ret += sprintf("\n%s received %3.2f%% of the experience.",who[i]->query_name(),share);
            exp = (float) xp * (float) share / (float) 100.00;
                who[i]->add_exp(round(exp));
        }
    }

    if(!cheating) {

        party_data[EXPERIENCE] += xp;
        party_data[KILLS] += 1;

        r_incr = add_rating(party,xp,num);

        party_data[DIRTY] = 1;
    }
    r_incr /= RATING_SCALE;
    ret = sprintf(":%d%s experience points (%s)%s",xp-bonus,(bonus ? sprintf(" (+%d bonus)",bonus) : ""),r_incr ? ""+r_incr : "<1",ret);
    party_shout(party,ret,"Sharing");
    if(cheating)
        party_shout(party,":thinks you should stop cheating.","The Party Daemon");
}


void notify_destruct() {
    save_party();
}

 

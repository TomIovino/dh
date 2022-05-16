////////////////////////////////////////
// house_d.c -- House Daemon
// 
// 970623  Third house_d for DragonHeart, Second by Geewhiz
//
#include <mudlib.h>
#include <ansi.h>
#include <bar.h>
#include <house.h>
#include <rank_d.h>

#define DEBUG_WIZARD_NAME "geewhiz"
#define NO_PRINTD
#define NO_ASSERT
#include <debug_utils.h>

#include <bim.h>


mapping house_data = ([ ]);
static mapping house_cache = ([ ]);
static mapping invited = ([ ]);

string fix_house(string house) {
    if(!house || !stringp(house))
        raise_error("house_d:  fix_house():  Bad argument 1\n");
    house = subst(lower_case(house)," ","_");
    house = subst(house,".","_");
    house = subst(house,"*","_");
    house = subst(house,">","_");
    house = subst(house,"<","_");
    house = subst(house,"|","_");
    house = subst(house,"~","_");
    house = subst(house,"%","_");
    house = subst(house,"!","_");
    house = subst(house,"?","_");
    house = subst(house,"'","_");
    house = subst(house,"\"","_");
    return house;
}    

string fix_house_name(string house) { return fix_house(&house); }

varargs string cap_words(string str, int blah) {
    if(!str || str == "")
        return str;
    return implode(map_array(explode(str," "),#'capitalize)," ");
}    

mapping new_house(string house, string creator) {
    mapping m;
    if(!house || !stringp(house))
        raise_error("house_d:  new_house():  Bad argument 1\n");
    if(!creator || !stringp(creator))
        raise_error("house_d:  new_house():  Bad argument 2\n");
    m = ([ ]);
    m[DIRTY] = 1;
    m[DISPNAME] = cap_words(house);
    m[NAME] = fix_house(house);
    m[CREATOR] = creator;
    m[MEMBERS] = ({ creator });
    m[DOB] = time();
    m[RATING] = 0;
    m[VERSION] = CURRENT_VERSION;
//PRINTD(m);
    return m;
}

static mapping static_data = new_house("**DEFAULT**","HouseDaemon");

varargs
string get_save_file(string house,int extension) {
    int c;
ASSERT(house != 0 && stringp(house));
    fix_house(&house);
    c = house[0];
    if(c < 'a' || c > 'z') c = '0';
    return sprintf("/ob_saves/housed/%c/%s%s",c,house,
          extension ? ".o" : "");
}

varargs
void save_house(int only_if_dirty) {
    if(!house_data || !mappingp(house_data) || !stringp(house_data[NAME]))
        return;
    if(only_if_dirty && !house_data[DIRTY]) return;
    house_data[DIRTY] = 0;
    save_object(get_save_file(house_data[NAME]));
}


static int IN_restore_house = 0;    // Allows restore_house() to be reentrant
static int IN_assign_new_leader = 0;

int *get_IN() { 
    return ({ IN_restore_house, IN_assign_new_leader });
}

void reset_IN() { 
   IN_restore_house =  IN_assign_new_leader = 0;
}
   

void CO_send_mail(string creator, string *all_members, string subject, string msg) {
    closure send_mail;
    int i;
    mapping mesg;
    mesg = ([ 
                SNDR : "HouseDaemon",
                RCPT : creator,
                CCOPY: implode(all_members - ({ creator }),", "),
                SUBJ : subject,
                MESSAGE : sprintf("%-=65s\n    -- The House Daemon\n",msg),
                DATE : time(),
                COMPLETE: 1
           ]);
    send_mail = symbol_function("send_mail",find_object(BIM_DIR "bimd"));
    i = sizeof(all_members);
    while(i--) {
        funcall(send_mail,all_members[i],mesg,SF_QUIETLY);
    }
    printf("Finished mailing house members.\n");
}

void send_mail(string creator, string *all_members, string subject, string mesg) {
    printf("Sceduling an update mailing...\n");
    call_out("CO_send_mail",0,creator,all_members,subject,mesg);
}

void update_house_version() {
    int size;
    string *cut;
    if((size = sizeof(house_data[MEMBERS])) > MAX_HOUSE_MEMBERS) {
        house_data[MEMBERS] -= ({ house_data[CREATOR] });
        house_data[MEMBERS] = ({ house_data[CREATOR] }) + house_data[MEMBERS];
        cut = house_data[MEMBERS][MAX_HOUSE_MEMBERS..];
        send_mail(house_data[CREATOR],house_data[MEMBERS],
             sprintf("House %O Too Large",house_data[DISPNAME]),
             sprintf("This mail is being sent to notify you that the house %O "
             "was too large.  The following %d member(s) have been dismissed "
             "automatically:\n  %s\n\nThis leaves the house with the following "
             "members:\n %s\n",house_data[DISPNAME],size-5,
             implode(map_array(cut,#'capitalize),", "),
             implode(map_array(house_data[MEMBERS][0..4],#'capitalize),", ")));
        house_data[MEMBERS] = house_data[MEMBERS][0..4];
        house_data[DIRTY] = 1;
    }
    while(house_data[HOUSEVER] != CURRENT_VERSION) {
        switch(house_data[HOUSEVER]) {
            case 0:
            default:
                /* Do nothing, always check house group size now so
                 * just bump up version number
                 */
                break;
        }
        house_data[HOUSEVER]++;
    }
}

status restore_house(string house) {
    int restored;
    mixed err;
    
    restored = 0;
    fix_house(&house);
ASSERT(house && stringp(house));
    if(IN_restore_house) {
        if(house_data[NAME] == house)
            return 1;
        return 0;
    }
    IN_restore_house = 1;


//PRINTD(house);
PRINTD(house_data);
//PRINTD(house_cache);

    if(house_data && mappingp(house_data)) {
        save_house(1);
        if(house_data[NAME] == house) {
            IN_restore_house = 0;
            return 1;
        }
        if(stringp(house_data[NAME])) {
            house_cache[house_data[NAME]] = house_data;
            house_cache[house_data[NAME]][TOUCHED] = time();
        }
    }

    if(house_cache && mappingp(house_cache) && house_cache[house]) {
        house_data = house_cache[house];
        house_cache[house][TOUCHED] = time();
        IN_restore_house = 0;
        return 1;
    }

    if(err = catch(restored = restore_object(get_save_file(house)))) {
        printf("*Could not restore house:\nErr = %-=70s",err);
    }
    
    if(restored) {
        update_house_version();
    }
    
    IN_restore_house = 0;
    return restored;
}        

void set_house_data(string house, string key, mixed val) {
ASSERT(house && stringp(house));
ASSERT(key && stringp(key));
    fix_house(&house);
    if(!restore_house(house))
        raise_error(sprintf("house_d:  set_house_data():  No such house %O\n",house));
    house_data[DIRTY] = 1;
    house_data[key] = val;
//PRINTD(house_data);
//PRINTD(key);
//PRINTD(val);
}

string get_house_display_name(string house) {
ASSERT(house && stringp(house));
    if(!restore_house(house))
        return house;
    return house_data[DISPNAME];
}        

string query_house_display_name(string house) {
    return get_house_display_name(house);
}

varargs
void remove_invite(string house, string name, int quiet) {
    object who;
    string pname;
ASSERT(house && stringp(house));
ASSERT(name && stringp(name));
    if(!invited || !mappingp(invited))
        return;
    fix_house(&house);
    if(!invited[house])
        return;
    pname = get_house_display_name(house);
    if(member(invited[house],name) != -1) {
        if(!quiet && (who = find_player(name)))
            tell_object(who,"\nYour invitation to the House " + pname + " has expired.\n");
        invited[house] -= ({ name });
        if(!sizeof(invited[house])) {
             invited[house] = 0;
             m_delete(invited,house);
        }
    }
}

int is_invited(string house, string name) {
ASSERT(house && stringp(house));
ASSERT(name && stringp(name));
    if(!invited || !mappingp(invited))
        return 0;
    fix_house(&house);
    if(!invited[house])
        return 0;
    return member(invited[house],name) != -1;
}

void invite(string house, string name) {
    object who;
    string pname;
ASSERT(house && stringp(house));
ASSERT(name && stringp(name));
    if(!invited || !mappingp(invited))
        invited = ([ ]);
    fix_house(&house);
    if(!invited[house])
        invited[house] = ({ });
    if(member(invited[house],name) == -1) {
        invited[house] += ({ name });
        pname = get_house_display_name(house);
        if(who = find_player(name))
            tell_object(who,sprintf("\n%-=75s\n",
             "You have been invited to join the House " + pname + ".  If you want to "
             "join this house type 'serve " + pname + "' within 60 seconds.\n"));
        call_out("remove_invite",60,house,name);
    }
}

        
            
void flush_house_mess() {
    string *keys;
    int i;
    call_out("flush_house_mess",300);

    reset_IN();

    if(house_cache && mappingp(house_cache)) {
        i = sizeof(keys = m_indices(house_cache));
        while(i--)         
            if(house_cache[keys[i]])
                if(house_cache[keys[i]][TOUCHED])
                    if((time() - house_cache[keys[i]][TOUCHED]) > 900)
                        m_delete(house_cache,keys[i]);
    }
}    
    


void reset(status arg) {
    if(arg) return;
    call_out("flush_house_mess",300);
    if(!house_data || !mappingp(house_data))
        house_data = ([  ]);
    if(!house_cache || !mappingp(house_cache))
        house_cache = ([ ]);
    if(restore_house("DEFAULT")) {
        static_data = house_data;
        house_data = ([ ]);
    } else {
        house_data = static_data;
        save_house();
        house_data = ([ ]);
    }
    
}



// FIXME:  don't restore house here.  We don't know if we actually need
//         it loaded or not.  maybe a file_size() check instead?
status house_exists(string house) {
    if(!house || !stringp(house))
        raise_error("house_d:  house_exists():  Bad argument 1\n");
    return file_size(get_save_file(house,1)) > 0;
}


mapping get(int i) {
    mapping m;
    switch(i) {
        case 0:
            m = house_data;
            break;
        case 1:
            m = house_cache;
            break;
        case 2:
            m = invited;
            break;
        default:
            raise_error("house_d:  get():  Bad argument 1\n");
            break;
    }
    return copy_mapping(m);
}
       

mapping get_house(string house) {
ASSERT(house && stringp(house));
    fix_house(&house);
    if(!restore_house(house))
        return 0;
    return house_data;
}


status add_house(string house, string creator) {
    string fixed;
ASSERT(house && stringp(house));
    fixed = fix_house(house);
    if(house_exists(fixed))
        return 0;
    restore_house(fixed);  // Clear out current house
    house_data = new_house(house, creator);
    save_house();
}

object *sort_members(string house);

varargs
void update_house_ranking(string house, int del);


status remove_house(string house) {
    object *who;
    int i;
ASSERT(house && stringp(house));
    if(!house_exists(house))
        return 0;
    if(i = sizeof(who = sort_members(house)))
        while(i--)
            if(who[i]) who[i]->set_house(0);
    update_house_ranking(house,1);
    house_cache = m_delete(house_cache,house_data[NAME]);
    house_data = 0;
    if(rm(get_save_file(house)+".o"))
        return 1;
    return -1;
}

string query_house_creator(string house) {
ASSERT(house && stringp(house));
    if(!restore_house(house))
        raise_error(sprintf("house_d:  query_house_creator():  No such house %O\n",house));
    return house_data[CREATOR];
}

status add_house_member(string house, string name);


status query_house_member(string house, string name) {
    int found;
ASSERT(house && stringp(house));
    if(!name || !stringp(name))
        raise_error("house_d:  query_house_member():  Bad argument 2\n");
    if(!restore_house(house))
        raise_error(sprintf("house_d:  query_house_member():  No such house %O\n",house));
//PRINTD(house_data[MEMBERS]);
    if(!house_data  
    || !mappingp(house_data))
        raise_error(sprintf("house_d:  query_house_member():  house_data = %O\n",house_data));
    if(!house_data[MEMBERS]
    || !pointerp(house_data[MEMBERS]))
        house_data[MEMBERS] = ({ });
    if(!found = (member((string *)house_data[MEMBERS],(string)name) != -1))
        if(name == query_house_creator(house)) {
            add_house_member(house,name);
            return 1;
        }
    return found;
}

status add_house_member(string house, string name) {
ASSERT(house && stringp(house));
    if(!name || !stringp(name))
        raise_error("house_d:  add_house_member():  Bad argument 2\n");
    if(!restore_house(house))
        raise_error(sprintf("house_d:  add_house_member():  No such house %O\n",house));
    if(query_house_member(house,name))
        return 0;
    house_data[MEMBERS] += ({ name });
    house_data[DIRTY] = 1;
    return 1;
}        

status remove_house_member(string house, string name) {
ASSERT(house && stringp(house));
    if(!name || !stringp(name))
        raise_error("house_d:  remove_house_member():  Bad argument 2\n");
    if(!restore_house(house))
        raise_error(sprintf("house_d:  remove_house_member():  No such house %O\n",house));
    if(!query_house_member(house,name)
    || name == query_house_creator(house))
        return 0;

   
    house_data[MEMBERS] -= ({ name });
    house_data[DIRTY] = 1;
    return 1;
}

string *query_house_members(string house) {
ASSERT(house && stringp(house));
    if(!restore_house(house))
        raise_error(sprintf("house_d:  query_house_members():  No such house %O\n",house));
    if(!house_data[MEMBERS] || !sizeof(house_data[MEMBERS])) {
        if(query_house_creator(house))
            add_house_member(house,query_house_creator(house));
        else
            return 0;
    }
    return house_data[MEMBERS];
}

int query_house_experience(string house) {
ASSERT(house && stringp(house));
    if(!restore_house(house))
        raise_error(sprintf("house_d:  query_house_experience():  No such house %O\n",house));
    return house_data[EXPERIENCE];
}

int query_house_rating(string house) {
ASSERT(house && stringp(house));
    if(!restore_house(house))
        raise_error(sprintf("house_d:  query_house_rating():  No such house %O\n",house));
    return house_data[RATING];
}

int query_house_age(string house) {
ASSERT(house && stringp(house));
    if(!restore_house(house))
        raise_error(sprintf("house_d:  query_house_age():  No such house %O\n",house));
    return house_data[DOB];
}


mapping query_house_data() { return copy_mapping(house_data); }

string fmt_names(string *names) { return implode(map_array(names,#'capitalize),", "); }


/*
varargs
void update_house_ranking(string house, int del) {
ASSERT(house && stringp(house));
    if(!restore_house(house))
        raise_error(sprintf("house_d:  update_house_rankig():  No such house %O",house));
// Rankings Board Stub /////////////////////////////////////////////
catch(RANK_D->update_ranking(!del ? R_HOUSE : -R_HOUSE,house_data[DISPNAME],
                                       house_data[RATING],
                                       house_data[KILLS],
                                       capitalize(house_data[CREATOR])));
////////////////////////////////////////////////////////////////////
}

*/

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
    return interactive(who)               // Not interactive? not here.
//        && previous_object()
        && this_player()
//        && environment(previous_object()) // Not in room with killer? not here.
        && environment(this_player()) // Not in room with killer? not here.
//        && present(who,environment(previous_object()));
        && present(who,environment(this_player()));
}

object *sort_members(string house) {
    int i, j, dbg;
    object *who, tmp;
    who = ({ });
    if(!house || !restore_house(house))
        return who;
    if(!j = sizeof(house_data[MEMBERS]))
        return who;
    for(i = 0; i < j; i++) {
//printf("A%d tmp = %O\n",++dbg,find_player(house_data[MEMBERS][i]));
        if(tmp = find_player(house_data[MEMBERS][i])) {
//printf("B%d tmp = %O\n",dbg,tmp);
//printf("B%d house = %O\n",dbg,tmp->query_house());
            if(house == (string) tmp->query_house()) {
//printf("C%d\n",dbg);
                who += ({ tmp, });
            }
        }
    }
    return sort_array(sort_array(who, #'by_level), #'by_interactive);
}

void notify_destruct() {
    mapping old_house_data;
    save_house();
    old_house_data = house_data;
    if(!static_data || !mappingp(static_data))
        static_data = new_house("**DEFAULT**","HouseDaemon");
    house_data = static_data;
    save_house();
    house_data = old_house_data;
}

void set_player_house(string name, string house) {
    if(!static_data["P_H"] || !mappingp(static_data["P_H"]))
        static_data["P_H"] = ([ ]);
    static_data["P_H"][name] = house;
}

void checkin_house(string house, string name) {
    write("Checking your house...");
    if(!restore_house(house)) {
        write("doesnt exist...");
        add_house(house,"");
        save_house();
        if(!restore_house(house)) {
            write("boggle! House still doesnt exist.\n\tAsk Geewhiz to 'plain it to you.\n");
            return;
        } else {
            write("added...");
        }
    } else
        write("exists...");
    write("membership...");
    if(!query_house_member(house,name)) {
        write("not a member...");
        add_house_member(house,name);
        if(!query_house_member(house,name)) {
            write("boggle! still not a member\n\tAsk Geewhiz to 'plain it to you.\n");
            return;
        } else {
            write("added...");
        }
    } else
        write("yes...");
    write("done.\n");
    set_player_house(name,house);
}
        

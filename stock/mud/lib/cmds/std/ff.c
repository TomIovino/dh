#include <finger.h>


/* finger.c (ash) */
// The major external daemon of the player file info this contains and
// retrieves data in a real time fashion. Should a player's house, rank
// kingdom etc change, the finger data will be accurate.
// further enhancements would be realtime spouse, pretitle ranks
// Ash

#define VALID   "/secure/valid_d"

#include <mudlib.h>
#include <bar.h>
#include <player.cfg>
#include <ansi.h>
#include <process.h>
#include <udp.h>
#include "/obj/stats.h"

#define HOUSE_D "/secure/house_d"

#define RANKD "secure/rank_d"
#define TPQSL this_player()->query_security_level()

#define wizardp(WHO) ((WHO) && (WHO)->query_security_level())
#define adminp(WHO) ((WHO) && (WHO)->query_security_level() >= 90)

#include "/secure/bim/user_exists.c"

object this_player();
string last_on_string();

string query_flag();
string title, pretitle, name;
string race, long_desc, avatar, stuff, profession, address;
string spouse, enter_room, called_from_ip, email, house, extra_info;
string ip_number;
int last_on,last_off,last_update,session;
string al_title, alignment, long_desc;
int create, money, savings,exp,master_exp;
string realname,who_string;
int member,wp;
int gender,  level, security_level;
string stats;
int last_pk,last_death;
int age;
status is_invis;
string new; 
static int is_on, is_invisible;
int noble;
string time_string(int sec);

string social_status();
mixed sex(mixed male, mixed female);

varargs string get_finger_str(string str, status intermud) {
    object ob;
    string tmp;
    string finger;
    string err; 
    int res;
    who_string = 0;
    wp = noble = last_pk = last_death = profession = member = 
    title = pretitle = name = race = long_desc = avatar = stuff = 
    spouse = enter_room = called_from_ip = email = house =
    extra_info = ip_number = last_on = last_off = session = al_title =
    alignment = long_desc = create = money = savings = exp =
    master_exp = realname = gender =  level = security_level = 
    stats = age = is_invis = new = last_update = address = 0;

    if(str && sscanf(str,"%s %s",str,tmp) != 2) {
    str = lower_case(str);
    ob = find_player(str);
    }
    if(ob) {
        ob->query_create();
        ob->save_me(1);
        is_on = 1;
        is_invisible = ob->query_invis() && !wizardp(this_player());
    } else {
        is_on = is_invisible = 0;
    }

    if(!user_exists(str)) {
        return " No player with that name exists.\n";
    }
     
    if(err = catch(res = restore_object(SAVE_WIZARD+str))) {
        return sprintf("Error restoring %O\nErr = %-=65s",str,err);
    } else if(!res && err = catch(res = restore_object(GET_SAVE_PLAYER(str)))) {
        return sprintf("Error restoring %O\nErr = %-=65s",str,err);
    } else if(!res && err = catch(res = restore_object(SAVE_FIRST+str))) {
        return sprintf("Error restoring %O\nErr = %-=65s",str,err);
    } else if(!res && err = catch(res = restore_object(SAVE_NO_BANISH+str))) {
        return sprintf("Error restoring %O\nErr = %-=65s",str,err);
    }

    if(!res) {
        return " No player with that name exists.\n";
    }

    if(last_update < 942980000){
    }


   session = (last_off-last_on)/60;
   if((int)VALID->is_valid_alignment(alignment) != 1) alignment == "";
   if((int)VALID->is_valid_race(race) != 1) race == "";
   if((int)VALID->is_valid_profession(profession) != 1) profession == "";
   if(race == "dragon") profession = "";
    if(!HOUSE_D->query_member(house,name)) house == "";
    if(house == "" || house == "commoner")
        house = 0;
    if(spouse == "" || spouse == "none")
        spouse = 0;
    if(race == "" || race == "none")
        race = 0;
    if(!house && pretitle != "Serf") pretitle = "Commoner";
    if(house) noble = 1; 
    finger = CYAN_F;
 if(who_string){
    finger += who_string+"\n";
  } else 
    finger += capitalize(name)+" "+capitalize(title)+"\n";
    finger += NORM;
    if(security_level && (query_mud_port() == 8000))
       finger += ("Contributor of level "+security_level+".\n");
    finger += social_status();
    if((age/43200)>99) 
        finger += sex("He","She")+ " is a veteran of over 100 days online.\n";
    else if(age/43200)
        finger += sex("He","She")+" has over "+age/43200+" days online.\n";
    if(member)
        finger += sex("He","She")+" is on the Council of Founders.\n";
    finger += NORM+CYAN_F+last_on_string();

 
   if(extra_info && extra_info != "")
        finger += "Plan:\n"+extra_info;
    if(address)
        finger += sex("His","Her")+" address is "+address+"\n";
    if(long_desc){
        finger += "\n"+long_desc+NORM+"\n";
     }


// test

    if(TPQSL) {
       new = MAILD->query_new_mail(name,1);
        finger += "Member  : "+member+", WP : "+wp+", ";
        finger += "LV : "+level+", M_EXP : "+master_exp+", ";
        finger += "$$ : "+ money+", BANK : "+savings+"\n";
        finger += "Enter   : "+enter_room+"       ";
        finger += "Mud Mail: "+new; 
        finger += "Created : "+ctime(create)+"   ";
        finger += "Email:"+email+"\n";
        finger += "ON      : "+ctime(last_on)+"   ";
        finger += "IP   :"+called_from_ip+"\n";
        finger += "OFF     : "+ctime(last_off)+"   ";
        finger += "IP#  :"+ip_number+"\n";
        finger += "CTIME   : "+ctime(time())+"\n";
        finger += "STR " +query_stat("strength"); 
    }
    finger += NORM;
    return PRINTBAR+"\n"+finger+PRINTBAR+"\n";

 return finger;
}


status main(string arg) {
    string who, mud, retval;
    if(!arg || arg == "")
    return notify_fail("finger:  Syntax:  finger <user>[@<mud>]\n"),0;
    if (arg && sscanf(arg, "%s@%s", who, mud) != 2) {
    return printf(get_finger_str(arg)),1;
    }
    if (retval = (string)INETD->send_udp(mud, ([
      REQUEST: "finger",
      SENDER: (string)this_player()->query_real_name(),
      DATA: who
    ]), 1)) {
    return notify_fail(retval),0;
    }
    return printf("inetd:  Request Transmitted\n"),1;
}

object this_player() { return efun::this_player() ? efun::this_player() : this_object();
}


string time_string(int sec) {
    string ret;
    if(catch(ret = (string) "/cmds/wiz/_sd"->time_to_str(sec)))
        return "<unknown error occurred>";
    return ret;
}

string last_on_string() {
   if(!last_on)
       return sex("He","She")+" has not been seen for a long time.\n";
   if( (time()-604800) > last_on)
       return sex("He","She")+" has not been online for over a week.\n";
   if( (time()-3024000) > last_on)
       return sex("He","She")+" has not been seen for a long time.\n";
   if( (last_on + 86400) > time())
       return sex("He","She")+" has been online sometime today.\n";
   if( (last_on + 166400) > time())
       return sex("He","She")+" was online sometime yesterday.\n";
   return sex("He","She")+" was last seen a few days ago.\n";
}

string query_flag() { if(security_level)            
                       return   "";
                       return   ""; // DISABLED
                      if( (last_pk > last_death) &&
                      last_pk+86000 > time())       
                       return   "Player carries a [PK] flag.\n";
                      if(last_death+86000 > time()) 
                       return   "Player carries a [SAFE] flag.\n";
                       return        ""; }



string social_status(){
    string ret;
    ret = NORM+CYAN_F+"";
        ret += this_object()->position_title();
    if(spouse){
        ret += sex("He","She")+" is married to "+capitalize(spouse)+".\n";
         }
    if(house){
        ret += sex("He","She")+" is a member of House "+capitalize(house)+".\n";
    }
   return ret+NORM+CYAN_F;
}

string position_title() {
    string ret;
    ret = B_CYAN_F+sex("He ","She ");
//  if(security_level) ret += "is an Immortal";
//  if(!security_level){
//  if(alignment != "") ret = ret +" "+capitalize(profession)+"";
    if(profession != "") ret = ret +"is a "+capitalize(profession)+"";
    if(race != "") ret = ret +" "+capitalize(race)+"";
//   }
            return ret+"."+" ("+wp+")\n"NORM+CYAN_F;
}

mixed sex(mixed male, mixed female) {
    return gender == 2 ? female : male;
}



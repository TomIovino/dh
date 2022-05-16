// Banish Daemon (v1.01)
// Geewhiz@DoD, Geewhiz@DragonHeart
// Copyright (C) 1996,1997,1998 Andrew T. Graham  All Rights Reserved
//////////
// $Id: banish_d.c,v 1.1 1998/03/03 23:50:23 atgraham Exp atgraham $
//////////
// ChangeLog:
//   $Log: banish_d.c,v $
//   Revision 1.1  1998/03/03 23:50:23  atgraham
//   Initial revision
//
//////////

#include <banish.h>

 
mapping BannedDomains = ([ ]);
 
status load_data();
status save_data();


void mail(string name, string pass);
 
void destruct(object obj) {
    obj->save_me(0);
    efun::destruct(obj);
}
 
void reset(int started) {
    if(!started) {
        if(!load_data()) {
            debug_message("banish_d: reset(): warning: No data\n");
        }
    } else {
        if(!save_data()) { 
            debug_message("banish_d: reset(): warning: Could not save\n");
        }
    }
}
 
#define DATA_FILE "/ob_saves/mudlib/banish"

status load_data() { return restore_object(DATA_FILE); }
status save_data() { return save_object(DATA_FILE),1; }
 
status BannedIPNumber(string IPNum);
status BannedIPName(string IPName);
 
string random_password();

static object player;

string name() {
    string n;
    if(player
    && n = (string) player->query_real_name())
        return capitalize(n);
    return "<unknown>";
}
 
status accept_connection(object who, mapping flags, closure cb) {
    string IPnumber; 
    string IPname;
    status IPnumOnly;
 
    if(!who || !objectp(who))
        throw("banish_d: accept_connection(): Bad argument 1 (requires object)");
    if(!flags || !mappingp(flags))
        throw("banish_d: accept_connection(): Bad argument 2 (requires mapping)");
    if(!cb || !closurep(cb))
        throw("banish_d: accept_connection(): Bad argument 3 (requires closure)");
 
    player = who;

    IPnumber = query_ip_number(who);
    IPname = lower_case(query_ip_name(who));
    IPnumOnly = (IPnumber == IPname);    

printf("!!!");
 
    if(IPnumOnly) 
        printf("You have connected from %s.\n",IPnumber);
    else
        printf("You have connected from %s (%s).\n",to_string(IPname),IPnumber);
 
   if(BannedIPNumber(IPnumber)){
      printf("You site has been banned, you must be a total freak.\n");
            destruct(who);
            return 1;         
    }

    if(flags[0] == F_PASSREQD ) {
        if(stringp(flags[1])) {
            printf("Registration Password Required: ");
            input_to("get_reg_pass", 0, flags, who, cb, 0, IPnumber, IPname);
            return 1;
        } else {
mail("Bug:  F_PASSREQD but passwd == NULL",
          sprintf("Player %O connected from %s (%s)\nNeeds password, none assigned...\n",name(),IPnumber,IPname));
          
            destruct(who);
            return 1;         
        }
    }
 
    if(flags[0] == F_NEWPLAYR) {    // New player
        printf("\nChecking to see if access from your site has been restricted...\n");
        if(BannedIPNumber(IPnumber) || BannedIPName(IPname)
           || who->query_name() == "Zihweeg") {
            flags[0] = F_PASSREQD;
            flags[1] = random_password();
mail(sprintf("New Player:  %O (F_PASSREQD)",name()),
          sprintf("Player %O connected from %s (%s)\nNeeds password: %O\n",name(),IPnumber,IPname,to_string(flags[1])));

            destruct(who);
            return 1;
        } else {
            flags[0] = F_APPROVED;
            printf("No access restrictions for your site exist.\n");
            funcall(cb);
            return 1;
        }
    } else {
       funcall(cb);
       return 1;  // Not New,  Not Banned,  Let 'em in.
    }
    // How did we get down here?
    return 0;
}
 
void get_reg_pass(string str, mapping flags, object who, closure cb, int cnt, string IPnumber, string IPname){
    cnt++;
   flags[5]++;
    if(cnt > 3) {
        printf("\nToo many tries!\n");
mail(sprintf("Player Tried Passwd Too Many Times:  %O ",name()),
          sprintf("Player %O connected from %s (%s)\nEntered incorrect password %d times (%d total)\n",name(),IPnumber,IPname,cnt,flags[5]));
        destruct(who);
        return;
    }
    if(!str || str == "") {
        printf("\nAborting.\n");
        destruct(who);
        return;
    }
    if(str != flags[1]) {
        printf("\nIncorrect Password, Retry.\n");
        printf("Registration Password Required: ");
        input_to("get_reg_pass", 0, flags, who, cb, cnt, IPnumber, IPname);
        return;
    }
    printf("\nWelcome To DragonHeart!  You will no longer need your Registration Password.\n");
    flags[0] = F_APPROVED;
    flags[1] = 0;

mail(sprintf("Player Approved:  %O (F_APPROVED)",name()),
          sprintf("Player %O connected from %s (%s)\nEntered correct password\n",name(),IPnumber,IPname));

    who->save_me(0);
    funcall(cb);
}
 
int UNbanIPnumber(string IPNum) {
    BannedDomains -= ([ IPNum ]);
    save_data();
    return 1;
}
int BanIPNumber(string IPNum) {
    BannedDomains += ([ IPNum: time() ]); 
    save_data();
    return 1;
}
 
int BanIPName(string IPName) {
    BannedDomains += ([ IPName: time() ]); 
    save_data();
    return 1;
}
 
// 192.232.22.3
// ============
// 192.232.22.3  0 *'s
// 192.232.22.*  1 *
// 192.232.*.*   2 *'s
 
string IPNumMask(string IPNum, int type) {
    string *parts;
 
    if(type)
        parts = explode(IPNum, ".");
 
    switch(type) {
        case 0:
            return IPNum;
            break;
        case 1:
            parts[3] = "*";
            return implode(parts, ".");
            break;
        case 2:
            parts[2] = "*";
            parts[3] = "*";
            return implode(parts, ".");
            break;
    }
}
 
status BannedIPNumber(string IPNum) {
    int i;
    i = 3;
    while(i--) {
        if(member(BannedDomains,IPNumMask(IPNum,i))) return 1;
    }
    return 0;
}
 
status BannedIPName(string IPName) {
    return member(BannedDomains, IPName);
}
 
string random_password() {
    string chars, password;
    int m, i, j;
    password = "      ";
    chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    m = strlen(chars);
    for(i = 0 ; i < 6 ; i++) {
        password[i] = chars[random(m)];
    }
    return password;
}

int UnBan(string who) { m_delete(BannedDomains, who); return 1; }

void show() {
    string *s;
    int i;
    s = m_indices(BannedDomains);
    i = sizeof(s);
    while(i--) {
        printf("%-20s : %s\n",s[i],ctime(BannedDomains[s[i]]));
    }
  }
  
  
void mail(string subj, string mesg) {
    catch("/secure/bim/obj2mail"->send_mail("BanishDaemon","geewhiz",({  }),subj,mesg,"\t-- BanishDaemon"));
}


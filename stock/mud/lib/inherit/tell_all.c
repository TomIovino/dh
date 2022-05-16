////////////////////////////////////////////////////////////
// TELL_ALL.C         // DRAGONHEART // GEEWHIZ.6.16.95   //
////////////////////////////////////////////////////////////

// tell_all(string mesg, string color)

#include <ansi.h>

void my_tell_object(object who, string mesg);
string strip_trailing_newlines(string mesg);

varargs void tell_all(string mesg, string color) {
    if(color
    && stringp(color))
        mesg = sprintf("%s%s" NORM "\n",color,strip_trailing_newlines(mesg));
     map_array(users(),#'my_tell_object,mesg);
    log_file("tellall",sprintf("%s %-=65s\n",timestamp(),sprintf("%O %s",this_object(),mesg)));
}

int see(object who) {
    return !who->query_security_level()
        || who->query_env_var("SEETELLALL");
}

void my_tell_object(object who, string mesg) {
    if(!who
    || !objectp(who)
    || !see(who))
        return;
    tell_object(who,mesg);
}

string strip_trailing_newlines(string mesg) {
    if(mesg
    && stringp(mesg))
        while(mesg[<1] == '\n')
            mesg = mesg[0..<2];
    return mesg;
}


// channels.h : General purpose channel daemon.
// Bubbs@Timewarp.
// v1.0 :    Jul/93 : Original coding.
// v1.1 :    Aug/93 : Various fixes.
// v1.2 : 14/Sep/93 : Restricted channels, and INET channels.
// v1.3 : 29/Nov/93 : Various tidy-ups, and more security.
// v1.4 :  3/Feb/94 : Various patches.
// v1.41: 21/Apr/94 : Compatibility upgrades for inetd 0.6.
// v1.42: 24/Apr/94 : Fixed bug in inet channel distribution.
// v1.5 : 27/Apr/94 : Inet channels now have privacy.
// v1.51: 12/Aug/94 : Lambda's now only created once: More efficient.
// v1.52:  2/Sep/94 : Restrictions now more flexible.
// v1.53: 24/Sep/94 : Made more inter-mudlib portable.
// v1.54: 22/Oct/94 : SUP_CHAN can now exclude certain channels.
// v1.55: 25/Nov/94 : "channels <chan> list all" now possible.
 
#include <channeld.h>
#include <levels.h> 
// How big the channel history is.
#define MAX_HISTORY  24
 
// The 'superior' channel. This listens to ALL other channels,
// except ones defined by check_sup_chan_excludes() (defined below).
#define SUP_CHAN     "*"
 
#if defined(SUP_CHAN)
// Give a user the choice to exclude certain channels from SUP_CHAN.
// This should be changed, depending on your mudlib.
// Given is Timewarp's way....
status check_sup_chan_exclude(object user, string chan) {
  string ch_excl;
  ch_excl = (string)user->query_env_var("*_exclude");
  return (ch_excl ? member_array(chan, explode(ch_excl, " ")) != -1 : 0);
  }
#endif   // SUP_CHAN
 
int alignment_channel(object user, string chan) {
    object who;
    int ret;
    if(user->query_security_level())
        return 1;
    ret = chan == (string) user->query_alignment();
    return ret;
}
int profession_channel(object user, string chan) {
    object who;
    int ret;
    if(user->query_security_level())
        return 1;
    ret = chan == (string) user->query_profession();
    return ret;
}

int race_channel(object user, string chan) {
    object who;
    int ret;
    if(user->query_security_level())
        return 1;
    ret =  chan == (string) user->query_race();
    return ret;
}


#define RESTRICTED ([\
 "divinity"  :  Q_LEVEL;    #'>=;    SEC1,\
 "wiz"       :  Q_LEVEL;    #'>=;    SEC1,\
 "admin"     :  Q_LEVEL;    #'>=;    SEC9,\
 "intermud"  :  Q_LEVEL;    #'>=;    SEC1,\
 "interwiz"  :  Q_LEVEL;    #'>=;    SEC1,\
 "intercode" :  Q_LEVEL;    #'>=;    SEC1,\
 "interadmin":  Q_LEVEL;    #'>=;    SEC8,\
 SUP_CHAN    :  Q_LEVEL;    #'>=;    SEC9,\
 ])
 
// The INET channels, their destinations, and privacy.
// The destinations (which should be in lower case) can be:
// "*"                        :  Send to all (except INET_EXCLUDES).
// "mud"                      :  Send to named mud.
// ({ "mud1", "mud2", ... })  :  Send to all muds in array.
// Valid privacy codes are: IC_PUBLIC, IC_RESTRICT and IC_PRIVATE.
#define INETCHANS ([\
 "intermud"  : "*";                       IC_PUBLIC  ,\
 "intercode" : "*";                       IC_PUBLIC  ,\
 "interadmin": "*";                       IC_PRIVATE ,\
 "interadm": "*";        IC_PRIVATE,\
 ])
 
// These muds are excluded from INET_CHANS "*" (all).
#define INET_EXCLUDES ({ \
 lower_case(LOCAL_NAME),\
 })
 
// The 'aliased' channels.
#define ABBREVIATIONS ([\
 "imud" : "intermud"  ,\
 "iwiz" : "interwiz"  ,\
 "icode": "intercode" ,\
 "iadm" : "interadmin",\
 "adm"  : "admin"     ,\
 ])
 

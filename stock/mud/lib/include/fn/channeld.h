// channeld.h : General purpose channel daemon.
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
 
#include <levels.h>
 
#if !defined(CHANNELD_H)
 
#define CHANNELD_H
 
// Where the channel daemon is to be found.
#define CHANNELD           "secure/channeld/channeld"
 
// A "level" function.  This basically defines the hierarchy of users.
#define Q_LEVEL            "query_security"
 
// The "wiz" level.  At what "level" are users reguarded as "wizards".
#define W_LEVEL            SEC1
 
// Version.
#define CHANNELD_VERSION   "1.55"
 
// Restrition position codes.
#define RS_FNC             0
#define RS_CMP             1
#define RS_VAL             2
 
// Inetchan position codes.
#define IC_MUDS            0
#define IC_PRIVACY         1
 
// Inetchan privacy codes.
// Inetchan privacy governs what muds can get history or
// channel listings.
#define IC_PUBLIC          0    // Every mud can.
#define IC_RESTRICT        1    // Just to muds on this channel can.
#define IC_PRIVATE         2    // No muds can.
 
// Two defines used for INETD capability.
#define CHANNEL            "channel"
#define COMMAND            "cmd"
 
#endif    // CHANNELD_H

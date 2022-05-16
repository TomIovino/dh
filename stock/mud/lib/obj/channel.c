#include <mudlib.h>
#include <ansi.h>
#define TP this_player()
// PL_CHANNEL.C -- Most (98%) of the Code below
//                 was written by Bubbs@TimeWarp.
 

// Added by Bubbs.  Channels system.  10/Aug/93
// Upgrades by Bubbs.  2/Sep/94.
 
// Inherit this into the player object.
// Call init_channels(level) to add the channel commands.
 
// Edit init_channels() to define the channels you want
// dispearsed to all users (including restricted ones).
 
// If you wish to hook channels up to a soul daemon of
// some kind, the best place is in channel_cmd() below.
 
#include <channeld.h>
 
#define SOULD "/secure/soul/emote_d"
 
status wizardp(object who) { return who && (who->query_security() >= 10); }
 
// Function declarations.
string query_alt_name();
 
mapping channels = ([ ]);
 
void set_channel(string chan, status arg) {
  if ((status)CHANNELD->check_restricted(chan) == 0)
   return;
  if(chan == "reset"){
       channels = ([]);
         return;
   }
  if(chan == "announce")
      arg = 1;
  channels[chan] = arg;
  return;
  }
 
mixed query_channel(string chan) {
  if (!chan) return copy_mapping(channels);
  return (member(channels, chan) ? channels[chan] : -1);
  }
 
status remove_channel(string chan) {
  if (member(channels, chan) == 0)
   return 0;
  if(chan == "announce")
   return 0;
  channels = m_delete(channels, chan);
  return 1;
  }

status add_channel(string chan) {
  if(query_channel(chan) == -1)
   return channels[chan] = 1;
  return 0;
}
 
// Generic 'channel' command.  Not "channels" command.
status channel_cmd(string str) {
  string chan;
  chan = query_verb();
  if(chan[0] == '-')
    chan = chan[1..];
  if(str != "-h")
  if(!wizardp(this_player()) && (chan == "announce" || chan == "info") )
      return 0;
  if(chan == "mortal" &&
      this_player()->query_level() < 10 && str != "-h" && str){
      write("Due to abuse you must be level 10 to use this channel.\n");
      return 0;
  } 
  if (member(channels, chan) == 0) return 0;
  if (!str)
   (void)CHANNELD->channel_list(chan);
  else if (str == "-h")
   (void)CHANNELD->channel_history(chan);
  else {
   if(this_player()->query_attrib("silenced")) return 0;

#if 1  // Add soul support here.
    if (str[0] == ';') {
      catch(str = (string)SOULD->get_feeling(str[1..],chan));
      if(str)
       str = sprintf(":%s", str);
      else
       return 0;
      }
    if (str[0] == '*') {
      catch(str = (string)SOULD->get_feeling(str[1..],chan));
      if(str)
       str = sprintf(":%s", str);
      else
       return 0;
      }
#endif
 
   

 if (channels[chan] == 0)
     channels[chan] = 1;
  //////// Filter out beeps ////////
  if(!wizardp(this_player()) && member(str,'\a') > -1) {
      str = subst(str,"\a","*BEL*");
      log_file("CHANNEL_BEEPS",sprintf("[%s] Chan:%-12s Message:%O this_player()==%O\n",ctime(),chan,str,this_player()));
      if(this_player())
         tell_object(this_player(),"\nGo *BEEP*\a your own horn!\n");
  }
  //////////////////////////////////  
    (void)CHANNELD->tell_channel(chan,
    capitalize(query_alt_name()), str);
    }
  return 1;
  }
 
// Initiate channel commands.
static void init_channels(int level) {
  string *all_channels;
  mapping o_chan;
  int i;
  if (!mappingp(channels)) channels = ([]); 
    
   all_channels = 
   ({
     "mortal",
   });

  if(!this_player()->query_security_level()) {
   all_channels =
   ({
     "wiz", "admin",      "mortal",
   });
// "intermud", "intercode", "interadmin",

    // For players...
    // Give all channels in all_channel list above.
    // Keep player's on/off setting for the channel if
    // they had it before... otherwise add it and turn
    // it on.  Remove all other channels.
  
    o_chan = ([ ]);
    i = sizeof(all_channels);
    while(i--) {
      if(member(channels,all_channels[i]))
        o_chan[all_channels[i]] = channels[all_channels[i]];
      else
        o_chan[all_channels[i]] = 1;
    }
    channels = o_chan;
  } else {
    // For Wizards
    // Do nothing
    // Wizards can add their own channels
    // Wizards should be able to permanently turn off
    // any channel they wish.
    i = sizeof(all_channels);
    while(i--) {
      // Add channel adds the channel and turns it on if
      // it doesnt exist... leaves it in its current state
      // otherwise.
      add_channel(all_channels[i]);
    }
  }

  channels["announce"] = 1;
  channels["info"] = 1;
  channels = filter_mapping(channels,
  symbol_function("check_channel", CHANNELD));
  if (this_player() == this_object())
      add_action("channel_cmd", "", 1);
  return;
}


// Bubbs@TimeWarp Channel Daemon v1.55
 
inherit "/secure/channeld/misc_simul.c";
#include <channeld.h>
 
void set_channel(string chan, status state, status nstate) {
  if (state == nstate)
   printf("Channel %s is already %s.\n", chan, nstate ? "on" : "off");
  else {
    (void)this_player()->set_channel(chan, nstate);
    printf("Channel %s is now %s.\n", chan, nstate ? "on" : "off");
    }
  }
 

void help();
status main(string str) {
  mapping channels;
  string chan, action, arg, *key;
  int index, size;
  string ret;
  
//if((string)this_player()->query_real_name() == "marcellus") return help(),1;

  if (str=="-h") return help(),1;
  channels = (mapping)this_player()->query_channel();
  if (!str) str = "all";
  if (sscanf(str, "%s %s", chan, action) != 2) {
    if (str == "all") {
      ret = "";
      key = sort_array(m_indices(channels),#'>);
      size = sizeof(key);
      write("Channels:\n");
      for(index = 0; index < size; index++)
       ret += sprintf("%-12s%2s\n",
        key[index], channels[key[index]] ? "y " : "n ");
      
      printf("%-#79.*s\n",size < 10 ? 1 : (size / 10),ret);
      return 1;
      }
    else if (member(channels, str) == 0)
     printf("No such channel: %s.\n", str);
    else
     printf("%-20s%s\n", str, channels[str] ? "on" : "off");
    return 1;
    }
  sscanf(action, "%s %s", action, arg) ;
  if (chan != "all" && member(channels, chan) == 0) {
    if ((int)call_other(this_player(), Q_LEVEL) > W_LEVEL &&
     (action == "add" || action == "a")) {
      (void)this_player()->set_channel(chan, 1);
      printf("Added channel %s.\n", chan);
      }
    else
     printf("No such channel : %s.\n", chan);
    }
  else switch(action) {
    case "on":
    case "y":
    case "off":
    case "n":
      if (chan == "all") {
        key = m_indices(channels);
        size = sizeof(key);
        for(index = 0; index < size; index++)
         set_channel(key[index], channels[key[index]],
          action == "on" || action == "y");
        }
      else 
       set_channel(chan,
        channels[chan], action == "on" || action == "y");
    break;
    case "history":
    case "h":
      if (chan == "all")
       write("Cannot perform 'history' on 'all'.\n");
      else if (!arg)
       (void)CHANNELD->channel_history(chan);
      else if (arg[0] == '@')
       (void)CHANNELD->get_inet_channel_history(chan, arg[1..]);
      else
       write("Unrecognised argument to 'history' command.\n");
    break;
    case "list":
    case "l":
      if (chan == "all") {
        key = m_indices(channels);
        size = sizeof(key);
        for(index = 0; index < size; index++)
         (void)CHANNELD->channel_list(key[index]);
        }
      else if (!arg)
       (void)CHANNELD->channel_list(chan);
      else if (arg[0] == '@') {
        key = explode(arg[1..], "@");
        size = sizeof(key);
        for(index = 0; index < size; index++)
         (void)CHANNELD->get_inet_channel_list(chan,
          trimstr(key[index]));
        }
      else if (arg == "all")
       (void)CHANNELD->get_inet_channel_all_list(chan);
      else
       write("Unrecognised argument to 'list' command.\n");
    break;
    default:
      if ((int)call_other(this_player(), Q_LEVEL) > W_LEVEL &&
       (action == "remove" || action == "r")) {
        printf((this_player()->remove_channel(chan) ?
         "Removed channel %s.\n" : "No such channel %s.\n"), chan);
        }
      else
       write("Usage: channels [<chan> [<action>]]\n");
    break;
    }
  return 1;
  }
 
/* status help() {
  write("Usage: channels [<chan> [<action> [<argument>]]]\n"
        "Control channels available.\n"
        "  'channels'                    - list channels.\n"
        "  'channels <chan>'             - give details on channel <chan>.\n"
        "  'channels <chan> <action>'    - perform action on channel <chan>.\n"
        "  'channels <chan> list @<mud>' - request remote channel listing.\n"
        "  'channels <chan> list all'    - request remote channel listings.\n"
        "By giving 'all' as a channel, the action is performed on all channels.\n"
        "Valid actions (with alias) are:\n"
        "  on      (y)    - turn channel on.\n"
        "  off     (n)    - turn channel off.\n"
        "  history (h)    - give last 20 messages on channel.\n"
        "  list    (l)    - list users listening to channel.\n");
  if ((int)call_other(this_player(), Q_LEVEL) > W_LEVEL)
  write("Extra wizard actions:\n"
        "  add     (a)    - add new channel.\n"
        "  remove  (r)    - remove channel.\n");
return 1;
  }
*/
 
// Following line added by Geewhiz@Dreamtime to make
// it compatible with Dreamtime
 
status _channels(string arg) { return main(arg); }

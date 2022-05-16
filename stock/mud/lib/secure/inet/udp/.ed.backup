#include <channeld.h>
#include <udp.h>

int udp_channel(mapping data) {
  string message;
  if (!data[CHANNEL]) data[CHANNEL] = data["CHANNEL"];
  if (!data[COMMAND]) data[COMMAND] = data["CMD"];
   if(data[CHANNEL][0..1] == "d-") return 1;
  switch(data[COMMAND]) {
    case "list":
      INETD->send_udp(data[NAME], ([
       REQUEST  : REPLY,
       RECIPIENT: data[SENDER],
       ID       : data[ID],
       DATA     : (string)CHANNELD->inet_channel_list(data[CHANNEL],
                                                      data[NAME]),
       ]));
    break;
    case "history":
      INETD->send_udp(data[NAME], ([
       REQUEST  : REPLY,
       RECIPIENT: data[SENDER],
       ID       : data[ID],
       DATA     : (string)CHANNELD->inet_channel_history(data[CHANNEL],
                                                         data[NAME]),
       ]));
    break;
    case "emote":
      message = sprintf("%s@%s %s",
       capitalize(data[SENDER]),
       capitalize(data[NAME]),
       data[DATA]);
      (void)CHANNELD->tell_inet_channel(data[CHANNEL],
                                        message,
                                        data[NAME]);
    break;
    default:
      message = sprintf("%s@%s : %s",
       capitalize(data[SENDER]),
       capitalize(data[NAME]),
       data[DATA]);
      (void)CHANNELD->tell_inet_channel(data[CHANNEL],
                                        message,
                                        data[NAME]);
    break;
    }
  return 1;
  }

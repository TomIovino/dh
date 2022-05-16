#include <udp.h>
#include <bim.h>
#include <bim_imud.h>

void udp_mail(mapping data) {
  if ((string)previous_object() != INETD)
   return;
  if (!member(data, RECIPIENT) || !data[RECIPIENT]) {
    log_file(INETD_LOG_FILE, "Invalid mail packet.  No Recipient.\n");
    return;
    }
  if (!(status)BIM_DIR"bimd"->valid_destination(data[RECIPIENT], 1)) {
    (string)INETD->send_udp(data[NAME], ([
     REQUEST        : REPLY,
     RECIPIENT      : data[SENDER],
     UDPM_STATUS    : UDPM_STATUS_UNKNOWN_PLAYER,
     UDPM_SENDER    : data[UDPM_SENDER],
     UDPM_SPOOL_NAME: data[UDPM_SPOOL_NAME],
     ID             : data[ID],
     DATA           : sprintf("Reason: Unknown player %O\n"
                              "\n"
                              "INCLUDED MESSAGE FOLLOWS :-\n"
                              "\n"
                              "Subject: %s\n"
                              "%s",
                       capitalize(data[RECIPIENT]),
                       data[UDPM_SUBJECT],
                       data[DATA]),
     ]));
    return;
    }
  (status)BIM_DIR"bimd"->send_mail(data[RECIPIENT], ([
   SNDR   : sprintf("%s@%s", data[UDPM_SENDER], capitalize(data[NAME])),
   RCPT   : data[RECIPIENT],
   SUBJ   : data[UDPM_SUBJECT],
   MESSAGE: data[DATA],
   DATE   : time(),
   ]), 0);
  (string)INETD->send_udp(data[NAME], ([
   REQUEST        : REPLY,
   RECIPIENT      : data[SENDER],
   UDPM_STATUS    : UDPM_STATUS_DELIVERED_OK,
   UDPM_SENDER    : data[UDPM_SENDER],
   UDPM_SPOOL_NAME: data[UDPM_SPOOL_NAME],
   ID             : data[ID],
   DATA           : data[RECIPIENT]
   ]));
  return;
  }

int clean_up(int arg) { destruct(this_object()); }

// BIM Mail Daemon: Intermud.
// Send and receive mail to/from the intermud.
// (Supported: Intermud-0.6/0.7 by Nostradamus@Zebedee).
// Spooling functions based on Alvin@Sushi's mail spooler.
// Bubbs.  6/Jun/95.

#include <udp.h>
#include <bim.h>
#include <bim_imud.h>


// Spooled mail, so we can save/restore.
mapping spool_item;

// (Static) closure for printing mud names nicely.
static closure print_muds;
// (Static) array of filenames in spool directory.
static string *spool = ({ });


// Forward declarations.
static void get_pending_deliveries();
static void save_spool_item();
static void remove_spool_item(string spool_file);
static void start_retry_callout();
static void failed_to_deliver(mapping data);


#if defined(COMPAT_MODE)
void reset(status arg) {
  if (arg) return;
#else
void create() {
#endif
  get_pending_deliveries();
  print_muds = lambda(({ 'lm, 'hosts }),
   ({ #'[,
     ({ #'[, 'hosts, 'lm }),
     HOST_NAME
   }));
  return;
  }


static void get_pending_deliveries() {
  string *entries;
  int i;
  entries = get_dir(UDPM_SPOOL_DIR"*.o");
  if (!entries || !sizeof(entries)) return;
  spool = map_array(entries, #'[..<], 0, 3);
  start_retry_callout();
  return;
  }


varargs void deliver_mail(string recipient, string mud, mapping mesg,
                          int mail_status, string spool_name) {
  string error;
  error = (string)INETD->send_udp(mud, ([
   REQUEST        : "mail",
   RECIPIENT      : recipient,
   SENDER         : this_object(),
   UDPM_SENDER    : mesg[SNDR],
   UDPM_SUBJECT   : mesg[SUBJ],
   DATA           : mesg[MESSAGE],
   UDPM_STATUS    : mail_status,
   UDPM_SPOOL_NAME: spool_name,
   ]), 1);
  return;
  }


void udp_reply(mapping data) {
  object sender;
  switch(data[UDPM_STATUS]) {
    case UDPM_STATUS_TIME_OUT:
      failed_to_deliver(data);
    break;
    case UDPM_STATUS_DELIVERED_OK:
      if (sender = find_player(data[UDPM_SENDER]))
       tell_object(sender,
        sprintf("Mailer@%s: Mail to %s delivered ok.\n",
         data[NAME],
         capitalize(data[DATA])));
      if (data[UDPM_SPOOL_NAME])
       remove_spool_item(data[UDPM_SPOOL_NAME]);
    break;
    case UDPM_STATUS_UNKNOWN_PLAYER:
      (status)BIM_DIR"bimd"->send_mail(data[UDPM_SENDER], ([
       SNDR   : sprintf("Mailer@%s", capitalize(data[NAME])),
       RCPT   : data[UDPM_SENDER],
       SUBJ   : "Bounced Mail",
       MESSAGE: data[DATA],
       DATE   : time(),
       ]), SF_QUIETLY);
      if (data[UDPM_SPOOL_NAME])
       remove_spool_item(data[UDPM_SPOOL_NAME]);
    break;
    case UDPM_STATUS_IN_SPOOL:
      // Do nothing.
    break;
    default:
      LOG(sprintf("Unknown value for UDPM_STATUS.  Data:\n"
                  "%O\n",
       data));
    break;
    }
  return;
  }


static void save_spool_item() {
  string name;
  int count;
  if (!mappingp(spool_item) || !sizeof(spool_item))
   return;
  while(member_array(name =
   sprintf("%s-%d", spool_item[UDPMS_DEST], count++), spool) != -1);
  save_object(sprintf(UDPM_SPOOL_DIR"%s", name));
  spool += ({ name });
  return;
  }


static void remove_spool_item(string spool_file) {
  string fname;
  int i;
  if (member_array(spool_file, spool) != -1)
   spool -= ({ spool_file });
  fname = sprintf("/"UDPM_SPOOL_DIR"%s.o", spool_file);
  if (file_size(fname) > 0 && !rm(fname))
   LOG(sprintf("Cannot delete spooled mail: %s\n", fname));
  return;
  }


static void start_retry_callout() {
  if (find_call_out("retry_send") != -1) return;
  call_out("retry_send", UDPM_RETRY_SEND * 60);
  return;
  }


static void retry_send() {
  int i, s;
  s = sizeof(spool);
  if (!s) return;
  for(i = 0; i < s; i++) {
    string savef;
    if (!restore_object(savef = sprintf(UDPM_SPOOL_DIR"%s", spool[i])))
     LOG(sprintf("Failed to restore spooled mail: %s\n", savef));
    else if (time() - spool_item[UDPMS_TIME] > UDPM_SEND_FAIL * 60) {
      (status)BIM_DIR"bimd"->send_mail(spool_item[UDPMS_FROM], ([
       SNDR   : "Mailer@"LOCAL_NAME,
       RCPT   : spool_item[UDPMS_FROM],
       SUBJ   : "Bounced Mail",
       MESSAGE: sprintf("Reason: Unable to connect to site \"%s\"\n"
                        "\n"
                        "INCLUDED MESSAGE FOLLOWS :-\n"
                        "\n"
                        "Recipient: %s\n"
                        "Subject: %s\n",
                 spool_item[UDPMS_DEST],
                 spool_item[UDPMS_TO],
                 spool_item[UDPMS_SUBJECT],
                 spool_item[UDPMS_BODY]),
       DATE   : time(),
       ]), SF_QUIETLY);
      remove_spool_item(spool[i]);
      }
    else
     deliver_mail(spool_item[UDPMS_TO],
                  spool_item[UDPMS_DEST],
                  ([
                   SNDR:    spool_item[UDPMS_FROM],
                   SUBJ:    spool_item[UDPMS_SUBJECT],
                   MESSAGE: spool_item[UDPMS_BODY]
                   ]),
                  UDPM_STATUS_IN_SPOOL,
                  spool[i]);
    }
  start_retry_callout();
  return;
  }


static void failed_to_deliver(mapping data) {
  object obj;
  if (!data[SYSTEM] || data[SYSTEM] != TIME_OUT) {
    LOG(sprintf("Bounced mail returning to: %O.\n",
     data[UDPM_SENDER]));    
    (status)BIM_DIR"bimd"->send_mail(data[UDPM_SENDER], ([
     SNDR   : "Mailer@"LOCAL_NAME,
     RCPT   : data[UDPM_SENDER],
     SUBJ   : "Bounced Mail",
     MESSAGE: sprintf("Reason: Error in connecting to remote site \"%s\"\n"
                      "\n"
                      "INCLUDED MESSAGE FOLLOWS :-\n"
                      "\n"
                      "Recipient: %s\n"
                      "Subject: %s\n"
                      "%s",
               data[NAME],
               data[RECIPIENT],
               data[UDPM_SUBJECT],
               data[DATA]),
     DATE   : time(),
     ]), SF_QUIETLY);
    return;
    }
  // Couldn't send it now.  Please in spool to send later...
  if (obj = find_player(data[UDPM_SENDER]))
   tell_object(obj,
    sprintf("Mail delivery to %s@%s timed out.  Placing mail in spool.\n",
     capitalize(data[RECIPIENT]), data[NAME]));
  spool_item = ([
   UDPMS_TIME   : time(),
   UDPMS_TO     : data[RECIPIENT],
   UDPMS_DEST   : data[NAME],
   UDPMS_FROM   : data[UDPM_SENDER],
   UDPMS_SUBJECT: data[UDPM_SUBJECT],
   UDPMS_BODY   : data[DATA]
   ]);
  save_spool_item();
  start_retry_callout();
  return;
  }


status valid_destination(string str, string error) {
  string name, mud;
  if (sscanf(str, "%s@%s", name, mud) == 2) {
    mapping hosts;
    string *tmp, lmud;
    status valid;
    tmp = m_indices(hosts = (mapping)INETD->query("hosts"));
    lmud = lower_case(mud);
    if (member_array(lmud, tmp) != -1)
     valid = 1;
    else {
#if INETD_VERSION < "0.7"
      tmp = filter_array(tmp,
       symbol_function("match_mud_name", INETD), lmud);
#else
      tmp = (string *)INETD->expand_mud_name(lmud);
#endif
      switch(sizeof(tmp)) {
        case 0:
          error = sprintf("Unknown mud: %s.\n",
           capitalize(mud));
        break;
        case 1:
          valid = 1;
          lmud = tmp[0];
        break;
        default:
          error = sprintf("Send mail to %s at which mud ?\n"
                          "%s.\n",
           capitalize(name),
           implode(map_array(tmp, print_muds, hosts), ", "));
        break;
        }
      }
    if (valid) {
      mud = hosts[lmud][HOST_NAME];
      str = sprintf("%s@%s", name, mud);
#if INETD_VERSION < "0.7" || 1
      tmp = hosts[lmud][HOST_COMMANDS];
      if (member_array("*",    tmp) == -1 &&
          member_array("mail", tmp) == -1)
#else
      if ((status)INETD->query("valid_request", "mail", lmud))
#endif
       error = sprintf("Sorry.  %s is unable to receive intermud mail.\n",
        mud);
      }
    return valid;
    }
  return 0;
  }

// Ok... BIMD has passed it to us to send to the intermud.
status send_mail(string rcpt, mapping mesg, int flags) {
  string whoto, mud;
  if (sscanf(rcpt, "%s@%s", whoto, mud) == 2) {
    if ((flags & SF_QUIETLY) == 0)
     printf("Sending mail to %s.  Please wait for confirmation.\n",
      capitalize(rcpt));
    deliver_mail(whoto, mud, mesg);
    return 1;
    }
  else {
    LOG(sprintf("bim_imud::send_mail(): Bad recipient: %O.\n", rcpt));
    return 0;
    }
  }

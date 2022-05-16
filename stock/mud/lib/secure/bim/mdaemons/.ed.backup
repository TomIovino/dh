// This is an example Mail Daemon.
// It is really very simple; if you send mail to it,
// it will send mail back to you, with the contents of
// INETD_HOSTS in the body.

// Bubbs.  29/May/95.

#include <bim.h>
#include <udp.h>


// There are only two functions necessary for a mail daemon.
// These are:
status valid_destination(string rcpt, string error);
status send_mail(string rcpt, mapping mesg);


// This is to find out what mail addresses this daemon recognises.
// It should return 1 if the address is recognised, 0 otherwise.
// However, if there is a problem, for example, a mud does not accept
// intermud mail, then set the error variable, which is passed
// by reference.
// The recipient argument is also passed by reference, but it
// should not be normally changed.  The daemon 'bim_imud.c' does
// change the recipient when expanding a mud name, for example.
status valid_destination(string rcpt, string error) {
  // In this case, we accept only a couple of fixed addresses.
  return (rcpt == "md_example" || rcpt == "host-list");
  if (rcpt == "md_example") return 1;
  }


// This is called when when an address recognised by
// valid_destination() is sent mail.
// The first argument is the address that is receiving the mail.
// The second argument is a mail message mapping.
// It will contain fields as listed near the end in bim.h.
// As a minimum, it will contain these fields:
// SNDR      The address that sent the mail.
// RCPT      The full recipient list of the mail.
// SUBJ      The subject of the mail.
// MESSAGE   The message body of the mail.
// DATE      The time (integer) that the mail was sent.
// In addition, these are the minimum fields required
// when sending mail.
// Once more, we return 1 for success, and 0 for failure.
status send_mail(string rcpt, mapping mesg) {
 // We wish to reply to this mail with the contents of INETD_HOSTS.
  (status)BIM_DIR"bimd"->send_mail(mesg[SNDR], ([
   SNDR   : "Example Mail Daemon",
   RCPT   : mesg[SNDR],
   SUBJ   : "Intermud Hosts List",
   MESSAGE: read_file(HOST_FILE),
   DATE   : time(),
   ]), SF_QUIETLY);
  return 1;
  }

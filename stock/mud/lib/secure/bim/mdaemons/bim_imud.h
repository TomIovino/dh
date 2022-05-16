// These defines are used in bim_inet.c, and the mail udp server.
// This file is more-or-less the same as it's cousin in
// Alvin@Sushi's package.

#if !defined(UDP_MAIL)

// So we don't have problems with double inclusion.
#define UDP_MAIL

// The directory in which mail is kept if the initial attempt times out.
// The mail will be resent every UDPM_RETRY_SEND minutes until
// UDPM_SEND_FAIL minutes have passed.
// NOTE: UDPM_SPOOL_DIR must end with / and must be writable.
#define UDPM_SPOOL_DIR                "data/mail/spool/"

// The following two times are in MINUTES.
#define UDPM_RETRY_SEND               30
#define UDPM_SEND_FAIL                1440     // 24 Hours.

// Don't change the following unless you like mayhem.

#define UDPM_SENDER                   "udpm_writer"
#define UDPM_SUBJECT                  "udpm_subject"
#define UDPM_STATUS                   "udpm_status"
#define UDPM_SPOOL_NAME               "udpm_spool_name"

// Status values.
#define UDPM_STATUS_TIME_OUT          0
#define UDPM_STATUS_DELIVERED_OK      1
#define UDPM_STATUS_UNKNOWN_PLAYER    2
#define UDPM_STATUS_IN_SPOOL          3

// Things to do with the spool directory.

#define UDPMS_TIME                    "udpm_spool_time"
#define UDPMS_TO                      "udpm_spool_to"
#define UDPMS_DEST                    "udpm_spool_dest"
#define UDPMS_FROM                    "udpm_spool_from"
#define UDPMS_SUBJECT                 "udpm_spool_subject"
#define UDPMS_BODY                    "udpm_spool_body"

#endif   // !defined(UDP_MAIL)

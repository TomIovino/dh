// BIM: Bubbs' Intermud Mailer.
// bim.h  :  The include file.
// This file is included in every file used in BIM.
// It contains a large number of options that need
// to be correctly configured for your mud.  Make sure
// that this file (or a copy) goes into a standard
// include directory.


#if !defined(BIM_H)

#define BIM_H

// DragonHeart requires....
#undef DATE

// Version string.
#define BIM_VERSION       "2.11"

// The main BIM directory.
#define BIM_DIR           "/secure/bim/"

// The groups file.
#define GROUPS_FILE "/ob_saves/mudlib/mail.dat"

// Array of Mail Daemons.
// Each filename in this list is a 'Mail Daemon', that is,
// it will handle mail destined for some external point.
// Two mail daemons come presupplied; md_example, and bim_imud.
// These are an example, and the intermud mail daemon, respectively.
// Add bim_imud only if you want intermud mail.
#define MAIL_DAEMONS      ({ BIM_DIR"mdaemons/bim_imud",    \
                             BIM_DIR"mdaemons/md_example" })

// The log file.
#define LOG(X)            log_file("BIM", X)

// A function to resolve pathnames.  Undefine if not available.
#define EVAL_PATH(P)      ((string)this_player()->make_path(P))

// A function to detect whether a user N exists.
// If unavailable, define it to 1.  But don't say I didn't warn you!
#define USER_EXISTS(N)    (status)BIM_DIR"user_exists"->user_exists(N)

// Use BIMs own editor and pager.  If you undefine this, you'll
// need to write your own initiation routines in bimr.c
#define BIM_EDITOR
#define BIM_PAGER

// User defines.

// Where the mail file for user N is kept.
#define MAIL_SAVE(N)      sprintf("usr/data/mail/%c/%s", (N)[0], (N))

// Can the user U use mail from anywhere ?  Or just from post offices.
#define MAIL_ANYWHERE(U)  (int)(U)->query_security_level()

// Should the user U be allowed to write a message to file ?
#define ALLOW_R_W_FILE(U) ((int)(U)->query_security_level() >= 10)

// Define which of the simulefuns from simuls.c you need.
#define SML_SET_OF
#define SML_SUBST
#define SML_SUM_ARRAY
#define SML_TRIMSTR

// How to obtain user U's name.
#define MNAME(U)          ((string)(U)->query_real_name())

// How to read user N's signiture.
#define READ_SIG(N)       read_file(sprintf("/w/%s/.sig", (N)))

// How to read user N's redirection string.
#define READ_REDIR(N)     read_file(sprintf("/w/%s/.forward", (N)))

// Should we auto-delete marked messages when we quit ?
// This also automatically deletes excess messages.
#define AUTO_DELETE(U)    (status)(U)->defined_env_var("delete_marked")

// Should we auto-mark messages once read ?
#define AUTO_MARK(U)      (status)(U)->defined_env_var("mark_read")

// Does the user U want the previous message body included
// with the 'reply' command.
// Settings are: yes, ask, no (default).
#define REPLY_INCLUDE(U)  "ask"

// Should the "x" command be quick-quit ?
// (that is, should it tidy, and quit)
#define X_QUICK_QUIT

// Should we allow the user to use 'Q': Quit without Save.
#define QUIT_NO_SAVE(U)   (int)((U)->query_security_level() ?  1 :  0)

// How many messages can the user U store ?
// 'Stored' messages are ignored by the message tally.
// Thus, this is the number of 'safe' messages.
// -1 means: no limit.
//#define STORE_QUOTA(U)    -1
#define STORE_QUOTA(U)    (int)((U)->query_security_level() ? -1 : 15)

// What is the maximum size we will allow the user U to have ?
// This limit does not include 'stored', 'new' or 'unread' messages.
// The excess messages are deleted before the user saves, so if
// you are setting a limit, make sure to deny the ability
// to quit without save.  (See the QUIT_NO_SAVE define, above).
// -1 means: no limit.
#define SIZE_QUOTA(U)     (int)((U)->query_security_level() ? -1 : 10)

// A definition for the default editor.
// What size of file is too big to read in.
#define FILE_TOO_BIG      80000

// A definition for the default pager.
// The default page_length to start with.
#define DEF_PAGE_LENG     20

// Internal BIM flag defines.
// Don't mess with these, unless you like chaos.
#define RCPT              "RCPT"        // Recipient.
#define SNDR              "SNDR"        // Sender.
#define S_MUD             "SMUD"        // Senders (S) Mud.
#define SUBJ              "SUBJ"        // Subject.
#define MESSAGE           "MESG"        // The message.
#define FLAGS             "FLGS"        // Bit-flags.
#define CCOPY             "CCPY"        // Carbon Copy.
#define DATE              "DATE"        // Date sent.
#define FWD_BY            "FWDBY"       // Forwarded by.
#define MAIL_GROUP        "MGRP"        // Mail group used.
#define COMPLETE          "CMPLT"       // Mail is complete.  Send now.
#define ORIG              "OR_"         // Original prefix.
#define DEFAULT           "DEF_"        // Default prefix.


// Internal Bit-Flags.
#define IF_NEW            0x001         // New message.
#define IF_UNREAD         0x002         // Unread message.
#define IF_MARKED         0x004         // Message marked (for deletion).
#define IF_KEPT           0x008         // Message retained.
#define IF_STORED         0x010         // Message stored.

// send_mail() Bit-flags.
#define SF_NO_REDIR       0x001         // Don't allow redirections.
#define SF_QUIETLY        0x002         // Make no noise.

// Some defines for the BIM editor.
#define ED_BUFF           "BFF"         // The edit buffer.
#define ED_MAILR          "MLR"         // The calling mailer (bimr).
#define ED_EXTRA          "XTR"         // Extra values.
#define ED_CURR           "CUR"         // The current line.
#define ED_Q_FUNC         "QTF"         // The quit-function.

// Some defines for the BIM pager.
#define PG_BUFF           "BFF"         // The edit buffer.
#define PG_MAILR          "MLR"         // The calling mailer (bimr).
#define PG_EXTRA          "XTR"         // Extra values.
#define PG_CURR           "CUR"         // The current line.
#define PG_Q_FUNC         "QTF"         // The quit-function.
#define PG_LENG           "LNG"         // Page length.
#define PG_SIZE           "SZE"         // Page buffer size.
#define PG_PATT           "PTT"         // Last search buffer.

// BIM options.

// Get the user U's default initiation command.
#define BIM_INIT_CMD(U)   (string)(U)->query_env_var("init_mail")

// The default initiation command.
#define DEFAULT_INIT_CMD  "list new"

// The prompt string.  Actually it's the last bit of the prompt.
#define BIM_PROMPT        ":"

// The default filename for when we write mail to a file.
#define DEFAULT_MAIL_FILE "mail.txt"

#endif   // BIM_H

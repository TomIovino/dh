// BIM: Bubbs' Intermud Mailer.
// _bim.c  :  The actual command.
// The function main() should be called when the user wants to
// access mail.  It should be passed the argument to the
// mail command.  The help() function will write the help
// for this command to the user.


#include <bim.h>

#if defined(SML_SET_OF) || defined(SML_TRIMSTR)
inherit BIM_DIR"simuls";
#endif

// Where everything starts.
status main(string str) {
  object bim;

/*
  if ((MAIL_ANYWHERE(this_player())) == 0 &&
   (status)environment(this_player())->query_post_office() == 0) {
    notify_fail("You must be in a post office to use mail.\n");
    return 0;
    }
*/

#ifdef 0
  write("Mail is currently disabled.\n");
  return 1;
#endif

  if(this_player()->query_attrib("silenced")) return 0;
  if(this_player()->query_age() < 3600){
        write("Due to abuse you must be one hour old before using mail.\n");
         return 1;
   }
  bim = (object)BIM_DIR"bimd"->query_mailer(this_player());
  if (!bim) {
    bim = clone_object(BIM_DIR"bimr");
    (void)bim->init_bim(str == 0);
    }
  else
   write("Located old mail object...\n");
  if (!str) {
    string init_cmd;
    init_cmd = BIM_INIT_CMD(this_player());
    (void)bim->parse_cmd(init_cmd ? init_cmd : DEFAULT_INIT_CMD);
    }
  else {
    mapping mesg;
    string *bits, tmp, *ccopy;
    int index, size;
    bits = explode(str, " -");
    size = sizeof(bits);
    mesg = ([ RCPT : bits[0] ]);
    for(index = 1; index < size; index++)
     switch(bits[index][0]) {
       case 's':    // Subject.
         tmp = trimstr(bits[index][1..]);
         if (tmp != "")
          mesg[SUBJ] = tmp;
       break;
       case 'c':    // Copy to.
         ccopy = explode(bits[index][1..], ",");
         ccopy = map_array(ccopy, #'trimstr);
         ccopy = set_of(ccopy) - ({ "" });
         if (sizeof(ccopy))
          mesg[CCOPY] = implode(ccopy, ", ");
         else
          mesg[CCOPY] = 0;
       break;
       case 'x':
         mesg[COMPLETE] = 1;
       break;
       case 'f':    // Insert file.
       case 'F':    // Insert file and subject.
         if (ALLOW_R_W_FILE(this_player())) {
           tmp = trimstr(bits[index][1..]);
           tmp = EVAL_PATH(tmp);
           if (file_size(tmp) <= 0)
            printf("File does not exist: %s.\n", tmp);
           else if (file_size(tmp) > 24000)
            printf("File too large: %s.\n", tmp);
           else {
             mesg[MESSAGE] = read_file(tmp);
             if (bits[index][0] == 'F')
              mesg[SUBJ] = tmp;
             }
           break;
           }
       default:
         printf("Unrecognised argument: '-%s'.\n", bits[index]);
       break;
       }
    (void)bim->build_mail(mesg);
    }
  return 1;
  }


// The help function.
void help() {
  if (ALLOW_R_W_FILE(this_player()))
   write("Usage: mail [<name>[@<mud>] [further options]]\n"
         "Invoke the mailer (BIM v"BIM_VERSION").\n"
         "Without an argument, this will drop you into the mailer\n"
         "menu, where you may use the full mail functions.\n"
         "Otherwise, build a mail message to <name>[@<mud>].\n"
         "The further options available are:\n"
         "    -s<subj>         Give the subject.\n"
         "    -c<copy_to>      Who to copy mail to.\n"
         "    -f<file>         Use <file> as the mail message.\n"
         "    -F<file>         As -f<file> and -s<file>.\n"
         "    -x               Send mail message without asking.\n"
         "\n"
         "See also: frm.\n");
  else
   write("Usage: mail [<name>[@<mud>] [further options]]\n"
         "Invoke the mailer (BIM v"BIM_VERSION").\n"
         "Without an argument, this will drop you into the mailer\n"
         "menu, where you may use the full mail functions.\n"
         "Otherwise, build a mail message to <name>[@<mud>].\n"
         "The available options are:\n"
         "    -s<subj>         Give the subject.\n"
         "    -c<copy_to>      Who to copy mail to.\n"
         "    -x               Send mail message without asking.\n"
         "\n"
         "Players must be in a post office to use this command.\n"
         "See also: frm.\n");
  return;
  }

int clean_up(int arg) { destruct(this_object()); }

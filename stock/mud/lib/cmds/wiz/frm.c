// BIM: Bubbs' Intermud Mailer.
// _frm.c  :  The 'frm' command.
// main() should be called when the "frm" command is executed.
// help() should be called for help on the "frm" command.


#include <bim.h>

// The main command.
status main() {
  object bim;
  bim = clone_object(BIM_DIR"bimr");
  (void)bim->init_bim(0);
  (void)bim->parse_cmd("list new");
  return 1;
  }


// The help function.
void help() {
  write("Usage: frm\n"
        "List new mail in your mailbox.\n"
        "See also: mail\n");
  return;
  }

int clean_up(int arg) { destruct(this_object()); }

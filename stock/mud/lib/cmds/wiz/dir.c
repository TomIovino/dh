// Bubbs.  21/Oct/94.

#include <ls.h>

object this_player() { return efun::this_player() ? efun::this_player() : this_object(); }

status main(string str) {
  string fspec, opts;
  int options;
  if (!str)
   str = "";
  str = trimstr(str);
  if (str[0] == '-') {
    if (sscanf(str, "-%s %s", opts, fspec) != 2)
      opts = str[1..], fspec = "";
    }
  else {
    opts = (string)this_player()->query_wiz_env_var("dir_opts");;
    fspec = str;
    }
  if (opts) {
    for(; opts != ""; opts = opts[1..])
     switch(opts[0]) {
       case 'a':  options |= LS_SHOW_HIDDEN;             break;
       case 'd':  options |= LS_SHOW_DIR;                break;
       case 'D':  options |= LS_DIRS_ONLY;               break;
       case 'f':  options |= LS_SHOW_FTIME;              break;
       case 'F':  options |= LS_FILES_ONLY;              break;
       case 'l':  options |= LS_SHOW_LOADED;             break;
       case 'L':  options |= LS_SHOW_LTIME;              break;
       case 'p':  options |= LS_PAGE_OUTPUT;             break;
       case 'r':  options |= LS_RECURSIVE;               break;
       case 's':  options |= LS_SHOW_FSIZE;              break;
       case 't':  options |= LS_SHOW_TOTALS;             break;
       case 'T':  options |= LS_SHOW_TTOTALS;            break;
       case 'w':  options |= LS_WIDE_FMT;                break;
       case '?':
         write("Usage: dir "USAGE"\n");
         options |= LS_NO_SHOW;
       break;
       case '0':  options |= 0;                           break;
       default :
         printf("dir: Unknown option '%c'.\n", opts[0]);
       break;
       }
    }
  else
   options = DIR_OPTIONS;
  if ((options & LS_NO_SHOW) == 0)
   (void)"cmds/wiz/ls"->do_ls(fspec, options);
  return 1;
  }

void help() {
  write("Usage: dir "USAGE"\n"
        "List the files in a directory.\n"
        "This command is now just an interface to the \"ls\" command.\n"
        "It's default options are \"adfstw\".  You may define your\n"
        "own default options by setting the \"dir_opts\" environmental\n"
        "variable.  For further help see the \"ls\" command.\n");
  return;
  }

int clean_up(int flag) { destruct(this_object()); }

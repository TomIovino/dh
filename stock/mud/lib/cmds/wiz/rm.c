#define WASTEBASKET  "/waste/"

#define O_FORCE      1
#define O_ASK        2

string *get_files(string str);
void do_ask_rm(string file, string *files, int options);
static status rm_file(string file, status force);

main(string str) {
  string *files, tmp;
  int index, size, options;
  if (this_interactive() != this_player()) {
    printf("Illegal (forced) attempt to 'rm %s' by %s.\n",
     str, capitalize((string)this_player()->query_real_name()));
    return 1;
    }
  if (!str)
   return
    notify_fail("Usage: rm [-dfin] <files>\n");
  // str = trimstr(str);
  if (sscanf(str, "-%s %s", tmp, str) != 2)
   tmp = "df";
  if (tmp) {
    for(; tmp != ""; tmp = tmp[1..])
     switch(tmp[0]) {
       case 'f': options |= O_FORCE;                            break;
       case 'n': options -= (O_FORCE & options);                break;
       case 'i': options |= O_ASK;                              break;
       case 'd': options -= (O_ASK & options);                  break;
  //   default : printf("rm: Unknown option '%c'.\n", tmp[0]);  break;
       default :
         return
          notify_fail(sprintf("rm: Unknown option '%c'.\n"
                              "rm: Failed.\n",
           tmp[0]));
       break;
       }
    }
  files = get_files(str);
  if ((options & O_ASK) == 0)
   for(size = sizeof(files); index < size; index++)
    printf(rm_file(files[index], options & O_FORCE) ?
     "rm: Removed %s\n" : "rm: Could not remove %s\n",
     files[index]);
  else if (sizeof(files)) {
    files = sort_array(files, #'>);
    do_ask_rm(files[0], files[1..], options);
    }
  return 1;
  }

string *get_files(string str) {
  string *fspecs, *files;
  int index, size;
  fspecs = map_array(explode(trimstr(str), " "), #'eval_path);
  size = sizeof(fspecs);
  files = ({ });
  for(index = 0; index < size; index++) {
    string dir, *tmp;
    tmp = get_dir(fspecs[index]);
    if (tmp && sizeof(tmp)) {
      tmp -= ({ ".", ".." });
      dir = implode(explode(fspecs[index], "/")[0..<2], "/");
      dir = trimstr(dir, '/');
      if (dir == "")
       files += tmp;
      else
       files += map_array(tmp, lambda(({ 'file }),
        ({ #'sprintf, "/%s/%s", dir, 'file })));
      files = filter_array(files, lambda(({ 'file }),
       ({ #'?,
         ({ #'==, -2, ({ #'file_size, 'file }) }),
         ({ #'printf, "rm: Cannot remove a directory: %s.\n", 'file }),
         ({ #'return, 1 }),
       })));
      files = sort_array(files, #'<);
      }
    else if (tmp)
     printf("rm: Cannot find file: %s.\n", fspecs[index]);
    else
     printf("rm: Cannot find directory: %s.\n", fspecs[index]);
    }
  return set_of(files);
  }

void do_ask_rm(string file, string *files, int options) {
  printf("Remove %s ? (y/N) ", file);
  input_to("get_ask_rm", 0, file, files, options);
  return;
  }

void get_ask_rm(string reply, string file, string *files, int options) {
  reply = lower_case(reply);
  if (reply == "q") return;
  if (reply == "y" || reply == "ye" || reply == "yes")
    printf(rm_file(file, options & O_FORCE) ?
     "rm: Removed %s.\n" : "rm: Could not remove %s.\n",
     file);
  if (sizeof(files))
   do_ask_rm(files[0], files[1..], options);
  return;
  }

static status rm_file(string file, status force) {
  // return (force ? rm(file) : (status)WASTED->waste_file(file));
  return rm(file);
  }

void help() {
  write("Usage: rm [-dfin] <files>\n"
        "Remove one or more files.\n"
        "On a 'normal' remove command, the file is not actually removed,\n"
        "but moved into "WASTEBASKET", which is cleared of files periodically.\n"
        "The options to this argument should be given all together, eg:\n"
        "> rm -fi r*.c\n"
        "There are 4 options:\n"
        "    n     'normal' remove  (move into "WASTEBASKET").\n"
        "    f     'forced' remove  (actually remove now).\n"
        "    i     Ask for confirmation before the remove.\n"
        "    d     Don't ask for confirmation.\n"
        "Note: Waste is temporarily offline.\n");
  return;
  }

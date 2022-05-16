// Bubbs.  12-13/Oct/94.
// Added recursive option.  27/Oct/94.
// Added -D/-F options.  10/Nov/94.

#include <ls.h>

varargs string trimstr(string str, int char, int mode) {
  string new;
  if (!str || !stringp(str)) return 0;
  if (str == "") return str;
  if (!char) char = ' ';
  if (!mode) mode = 3;
  new = str;
  if (mode & 1)
   while(new && new != "" && new[0] == char)
    new = new[1..];
  if (mode & 2)
   while(new && new != "" && new[<1] == char)
    new = new[0..<2];
  return new;
  }

private static string ls_time(int time) {
  string t;
  t = ctime(time);
  return sprintf("%s %s/%s/%s",
   t[11..15], t[8..9], t[4..6], t[22..23]);
  }

varargs string ls_string(string path, int flags, int t_d, int t_b) {
  mixed *data;
  object *obs;
  string directory, *outp, *files, *dirs, output;
  int index, size, bytes;
  if (file_size(path = (string)this_player()->make_path(path)) == -2)
   path = sprintf("%s/", path);
  data = get_dir(path, GD_FILES | GD_SIZES | GD_TIMES);
  if (!sizeof(data))
   return (data ? "No files found.\n" : "Directory not found.\n");
  outp = ({ });
  dirs = ({ });
  directory = implode(explode(path, "/")[0..<2], "/");
  size = sizeof(data) / 3;
  obs = allocate(size);
  files = allocate(size);
  outp = allocate(size);
  for(index = 0; index < size; index++)
   if ((flags & LS_DIRS_ONLY  ? data[3*index + 1] == -2 :
       (flags & LS_FILES_ONLY ? data[3*index + 1] != -2 : 1)) &&
       (data[3*index][0] != '.' || flags & LS_SHOW_HIDDEN)) {
     files[index] = sprintf("%s/%s", directory, data[3*index]);
     if ((flags & (LS_SHOW_LOADED | LS_SHOW_LTIME)) &&
      files[index][<2..] == ".c")
       obs[index] = find_object(files[index]);
     outp[index] = sprintf("%c%s%c",
      ((flags & LS_SHOW_LOADED) && obs[index] && data[3*index+1] != -2 ?
       '*' : ' '),
      data[3*index],
      ((flags & LS_SHOW_DIR) && (data[3*index+1] == -2) ? '/' : 0));
     if (data[3*index+1] == -2) dirs += ({ data[3*index] });
     else bytes += data[3*index+1];
     if (flags &
        (LS_SHOW_FTIME | LS_SHOW_FSIZE | LS_SHOW_LTIME | LS_WIDE_FMT)) {
       outp[index] = sprintf("%-20s", outp[index]);
       if (flags & LS_SHOW_FSIZE)
        outp[index] = sprintf("%s    %6s",
         outp[index],
         data[3*index+1] == -2 ? "<dir>" : to_string(data[3*index+1]));
       if (flags & LS_SHOW_FTIME)
        outp[index] = sprintf("%s    %15s",
         outp[index],
         ls_time(data[3*index+2]));
       if (flags & LS_SHOW_LTIME)
        outp[index] = sprintf("%s    %15s",
         outp[index],
         obs[index] ? ls_time(program_time(obs[index])) : "");
       }
     }
  output = sprintf(flags &
   (LS_SHOW_FTIME | LS_SHOW_FSIZE | LS_SHOW_LTIME | LS_WIDE_FMT) ?
    "%s\n" : "%-#80s\n",
    implode(outp - ({ 0 }), "\n"));
  t_d += (index = sizeof(dirs));
  t_b += bytes;
  if (flags & LS_SHOW_TOTALS)
   output = sprintf("%s      %d director%s.    %d byte%c.\n",
    output,
    index, index == 1 ? "y" : "ies",
    bytes, bytes == 1 ?  0  : 's');
  if (flags & LS_RECURSIVE) {
    size = sizeof(dirs);
    for(index = 0; index < size; index++)
     output = sprintf("%s\n%s:\n%s",
      output,
      dirs[index] = sprintf("%s/%s", directory, dirs[index]),
      ls_string(dirs[index], flags, &t_d, &t_b));
    }
  return output;
  }

varargs void do_ls(string path, int flags) {
  string *bits, output;
  int t_d, t_b;
  output = (flags & LS_SHOW_CWD ?
   sprintf("Current Path: /%s/\n",
    (string)this_player()->query_path()) :
    "");
  for(bits = explode(path, " "); sizeof(bits); bits = bits[1..])
   output = sprintf("%s%s", output, ls_string(bits[0], flags, &t_d, &t_b));
  if (flags & LS_SHOW_TTOTALS)
   output = sprintf("%s      %d director%s.    %d byte%c.\n",
    output,
    t_d, t_d == 1 ? "y" : "ies",
    t_b, t_b == 1 ?  0  : 's');
  if (flags & LS_PAGE_OUTPUT && 0) {
    object pager;
    int index;
#define PAGER "obj/pager"
    pager = clone_object(PAGER);
    (void)pager->set_quit_func(this_object(), "quit_pager");
    index = to_int((string)this_player()->query_env_var("page_opts"));
    if (index)
     (void)pager->set_options(index);
    index = to_int((string)this_player()->query_env_var("page_leng"));
    if (index)
     (void)pager->set_page_length(index);
    (void)pager->more_text(explode(output, "\n")[0..<2],
     sprintf("Directory: %s", path));
    }
  else if ((flags & LS_NO_SHOW) == 0)
   write(output);
  return;
  }

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
    opts = (string)this_player()->query_wiz_env_var("ls_opts");;
    fspec = str;
    }
  if (opts) {
    for(; opts != ""; opts = opts[1..])
     switch(opts[0]) {
       case 'a':  options |= LS_SHOW_HIDDEN;             break;
       case 'c':  options |= LS_SHOW_CWD;                break;
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
         write("Usage: ls "USAGE"\n");
         options |= LS_NO_SHOW;
       break;
       case '0':  options = 0;                           break;
       default :
         printf("ls: Unknown option '%c'.\n", opts[0]);
       break;
       }
    }
  else
   options = DEFAULT_OPTIONS;
  if ((options & LS_NO_SHOW) == 0)
   do_ls(fspec, options);
  return 1;
  }

void help() {
  write("Usage: ls "USAGE"\n"
        "List the files in a directory.\n"
        "There are a number of options available.  The default\n"
        "options list is \"d\".  You can set you own default\n"
        "options, with the \"ls_opts\" environmental variable.\n"
        "The options, and their actions are:\n"
        "    a    Show 'hidden' files (name begins with a '.').\n"
        "    c    Show the current working directory seperately.\n"
        "    d    Place a '/' after each directory.\n"
        "    D    Show directories only.  (-F overridden)\n"
        "    f    Show the time last modified.  (-w is assumed)\n"
        "    F    Show files only.\n"
        "    l    Place a '*' before each loaded object.\n"
        "    L    Show the time loaded.  (-w is assumed)\n"
        "    p    Run the output through the pager.\n"
        "    r    Recurse through subdirectories.\n"
        "    s    Show the file size.  (-w is assumed)\n"
        "    t    Show size totals.\n"
        "    T    Show total of size totals.\n"
        "    w    Output is in wide format.\n"
        "    0    Clear out any previously selected options.\n"
        "    ?    Print command usage.\n"
        "\n"
        "See also: dir.\n");
  return;
  }

void quit_pager(string reason, object pager, mixed extra) {
  destruct(pager);
  write(reason == "END" ? "<End of files>\n" : "Ok.\n");
  return;
  }

int clean_up(int arg) { destruct(this_object()); }

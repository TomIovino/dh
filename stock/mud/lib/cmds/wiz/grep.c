 // grep command.
 // Bubbs@Timewarp.   10/May/94.
 // Fixed filespec stuff and added -r flag.  Bubbs  28/May/94.

#include <mudlib.h>

 // Grep usage.
#define USAGE "grep [-cilnvxrp] <pattern> <file> [> <outfile>]\n"

 // How much of a file to read in at once.
#define READ_CHUNK   200

 // Options.
#define GR_COUNT       0x001     // -c
#define GR_LOWER       0x002     // -i
#define GR_LIST        0x004     // -l
#define GR_LINE_NUM    0x008     // -n
#define GR_NOT_MATCH   0x010     // -v
#define GR_EXACT       0x020     // -x
#define GR_RECURSIVE   0x040     // -r
#define GR_PAGE_OUTP   0x080     // -p

#define GD_FILES  1
#define GD_SIZES  2

static closure do_search;

void reset(status arg) {
  if (!do_search)
   do_search = lambda(({ 'line, 'search }),
    ({ #'!=, ({ #'strstr, 'line, 'search }), -1 }));
  }

string grep_file(string file, string search, int options) {
  string outp, txt, *text, *test;
  int index, size, *results;
  text = ({ });
  index = 0;
  outp = "";
  while(txt = read_file(file, 1 + READ_CHUNK * (index++), READ_CHUNK)) {
    text += explode(txt, "\n");
    if (text[<1] == "")
     text = text[0..<2];
    }
  if (options & GR_LOWER) {
    test = map_array(text, #'lower_case);
    search = lower_case(search);
    }
  else
   test = text;
  size = sizeof(test);
  if (options & GR_EXACT)
   results = map_array(test, #'==, search);
  else
   results = map_array(test, do_search, search);
  if (options & GR_LIST)
   outp = (sizeof(results - ({ 0 })) ?
    sprintf("%s\n", file) : "");
  else if (options & GR_COUNT)
   outp = sprintf("%s:%d\n", file, sizeof(results - ({ 0 })));
  else
   for(index = 0; index < size; index++)
    if (((options & GR_NOT_MATCH) >> 4) ^ (results[index]))
     outp = sprintf("%s%s:%s%s\n",
      outp,
      file,
      (options & GR_LINE_NUM) ? sprintf("%d:", index+1) : "",
      text[index]);
  return outp;
  }

string *get_files(string fspec, int options) {
  mixed *fdata;
  string dir, *files;
  int index, size;
  dir = implode(explode(fspec, "/")[0..<2], "/");
  fdata = get_dir(fspec, GD_FILES | GD_SIZES);
  if (!fdata) return 0;
  files = ({ });
  for(index = sizeof(fdata) - 1; index > 0; index -= 2) {
    string tmpfile;
    tmpfile = fdata[index - 1];
    if (tmpfile != "." && tmpfile != "..") {
      tmpfile = sprintf("%s/%s", dir, tmpfile);
      if (fdata[index] == -2 && (options & GR_RECURSIVE))
       files += get_files(sprintf("%s/", tmpfile), options);
      else if (fdata[index] != -2)
       files += ({ tmpfile });
      }
    }
  return files;
  }

main(str) {
  mixed *fdata;
  string *files, opts, search, fspec, outp, outfile;
  int options, i;
  if (!str)
   return
    notify_fail("Usage: "USAGE);
  if (sscanf(str, "-%s %s", opts, str) != 2)
   opts = (string)this_player()->query_wiz_env_var("grep_opts");
  if (sscanf(str, "\"%s\" %s", search, fspec) != 2 &&
   sscanf(str, "%s %s", search, fspec) != 2)
   return
    notify_fail("Usage: "USAGE);
  if (sscanf(fspec, "%s > %s", fspec, outfile) == 2)
   outfile = this_player()->make_path(outfile);
  if (opts) {
    for(; opts != ""; opts = opts[1..])
     switch(opts[0]) {
       case 'c': options |= GR_COUNT;                             break;
       case 'i': options |= GR_LOWER;                             break;
       case 'l': options |= GR_LIST;                              break;
       case 'n': options |= GR_LINE_NUM;                          break;
       case 'v': options |= GR_NOT_MATCH;                         break;
       case 'x': options |= GR_EXACT;                             break;
       case 'r': options |= GR_RECURSIVE;                         break;
       case 'p': options |= GR_PAGE_OUTP;                         break;
       default : printf("grep: Unknown option '%c'.\n", opts[0]); break;
       }
    }
  fspec = this_player()->make_path(fspec);
  if (file_size(fspec) == -2 && fspec[<1] != '/')
   fspec = sprintf("%s/", fspec);
  files = get_files(fspec, options);
  if (!files)
   return
    notify_fail("grep: Could not find any files.\n");
  outp = implode(map_array(files, #'grep_file, search, options), "");
  if (outfile) {
    if ((status)this_player()->valid_write(outfile) == 0)
     return
      notify_fail("grep: Cannot write output to file.\n");
    write_file(outfile, outp);
    printf("grep: Search for %O over %s written to %s.\n",
     search, fspec, outfile);
    }
  else if (options & GR_PAGE_OUTP) {
    object pager;
    pager = clone_object(PAGER);
    (void)pager->set_quit_func(this_object(), "quit_pager");
    i = to_int((string)this_player()->query_wiz_env_var("page_opts"));
    if (i)
     (void)pager->set_options(i);
    i = to_int((string)this_player()->query_wiz_env_var("page_leng"));
    if (i)
     (void)pager->set_page_length(i);
    (void)pager->more_text(explode(outp, "\n")[0..<2],
     sprintf("grep: %O over %s", search, fspec));
    }
  else
   write(outp);
  return 1;
  }

void help() {
  write("Usage: "USAGE
        "This command searches for a particular pattern in one\n"
        "or a group of files.  The pattern can be 'protected'\n"
        "by enclosing it in quotes (eg: \"  return;\").  Output\n"
        "can be redirected to file by the \"> <outfile>\" syntax.\n"
        "Wildcards can be passed for the <file> argument.\n"
        "The options, and their actions are as follows:\n"
        "    c    Show a count of matches made\n"
        "    i    Ignore case differences in comparisons\n"
        "    l    List only those files that contain matches\n"
        "    n    Print line number information\n"
        "    v    Print the lines that do not contain a match\n"
        "    x    Print only the lines that match exactly\n"
        "    r    Grep through all subdirectories found.\n"
        "    p    Run the output through the pager.\n");
  return;
  }

void quit_pager(string reason, object pager, mixed extra) {
  destruct(pager);
  write(reason == "END" ? "<End of grep>\n" : "Ok.\n");
  return;
  }

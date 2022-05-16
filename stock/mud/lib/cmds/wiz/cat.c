#define DEFAULT_END 50
#define CAT_LENGTH 45

        /* cat whole file */

Cat_file(string str) {
  int line;

  str = this_player()->make_path(str);
  if((string)this_player()->valid_read(str) == 0) 
    return notify_fail("Can not read file: "+ str +"\n");
  for(line = 1; cat(str, line, CAT_LENGTH); line += CAT_LENGTH);
  return 1;
} 

main(string str) {
  string file;
  int start, end, fsize;
  if (!str)
   return notify_fail("Usage: cat [-s <num>] [-e <num>] <file> -or- cat [-a] <file>\n");
  if(sscanf(str, "-a %s",file)) {
    Cat_file(file);
    return 1;
  }
  sscanf(str, "-s%d%s", start, str);
  end = DEFAULT_END - start;
  if (sscanf(trimstr(str), "-e%d%s", end, file) != 2)
   file = str;
  file = this_player()->make_path(trimstr(file));
  if ((string)this_player()->valid_read(file) == 0)
   return
    notify_fail("cat: Cannot read file.\n");
  if ((fsize = file_size(file)) == -1)
   return
    notify_fail("cat: File does not exist.\n");
  if (fsize == -2)
   return
    notify_fail("cat: Cannot cat a directory.\n");
  notify_fail("cat: No lines.\n");
  return cat(file, start, end);
  }

void help() {
  write("Usage: cat [-s <num>] [-e <num>] <file> -or- cat [-a] <file\n"
        "Print the contents of <file> to the screen.\n"
        "The '-s' and '-e' options dictate at which\n"
        "lines to start and finish printing the file.\n"
        "The default values are 0 and 50 respectively.\n"
        "The '-a' option will cat an entire file. This\n"
        "option is good for logging files to your home\n"
        "account via screen output.\n");
  return;
  }

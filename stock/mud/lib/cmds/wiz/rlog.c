status main(string str) {
  str = sprintf("/log/%s",
   str ? str : (string)this_player()->query_real_name());
  if(file_size(str)==-1) {
    write("Log file not found.\n");
  } else {
    "cmds/wiz/_rm"->main(str);
  }
  return 1;
}

void help() {
  write("Usage: rlog [<file>]\n\
Remove a named file from /log.\n\
The default for <file> is <your_name>.\n");
  }

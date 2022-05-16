status main(string str) {
  str = sprintf("/log/%s",
   str ? str : (string)this_player()->query_real_name()+"");
     if(!tail(str))
       if(this_player()->valid_read(sprintf(str))) {
         if(!tail(sprintf(str))) write("Log file empty.\n");
       }
       else
         write("Log file empty.\n");
  return 1;
  }

void help() {
  write("Usage: log [<file>]\n\
Display the end of the logfile passed as argument.\n\
Giving no file assumes your error log file.\n");
  }

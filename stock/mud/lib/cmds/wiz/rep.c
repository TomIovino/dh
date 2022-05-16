status main(string fun) {
  string str;

  str = "/log/" + this_player()->query_real_name() + ".rep";

   if(file_size(str)) {

   write("Log file not found. ( "+str+" )\n");

   } else {

    "cmds/wiz/_more"->main(str);

   }
  return 1;
}



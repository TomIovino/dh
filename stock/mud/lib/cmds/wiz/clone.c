main(string str) {
  string *files, mesg;
  int index, size;
  status silently;
  if (!str)
   return
    notify_fail("Usage: clone [-s] <file> [<extra files>]\n");
  if (str[0..2] == "-s ") {
    silently = 1;
    str = str[3..];
    }
  files = explode(str, " ");
  size = sizeof(files);
  if (!silently)
    mesg = this_player()->parse_msg(this_player()-> query_mclone() + "\n");
  for(index = 0; index < size; index++) {
    object clone;
    string error;
    files[index] = eval_path(files[index]);
    if (!((string)this_player()->valid_read(files[index])))
     write("Invalid file.\n");
    else {
      error = catch(clone = efun::clone_object(files[index]));
      if (error)
       printf("Error in cloning \"%s\": %s", files[index], error[1..]);
      else {
        string sh;
        sh = (string)clone->short();
        printf("You clone %s.\n", sh ? sh : tidy_file(clone));
        if (!sh) sh = (string)clone->query_name();
        if (!sh) sh = "something";
        if (!silently)
         say(sprintf("%s\n", subst(mesg, "%O", sh)));
        transfer(clone,
         (status)clone->get() ? this_player() : environment(this_player()));
        }
      }
    }
  return 1;
}

void help() {
  write("Usage: clone [-s] <file> [<extra files>]\n"
        "Clone an object, to either your inventory, or your\n"
        "environment (depending on whether it is get-able.)\n"
        "Giving extra files clones more than one object.\n"
        "If the \"-s\" option is given, no messages are\n"
        "given to the room.\n");
  return;
}

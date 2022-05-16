status main(string str) {
  if (!str) {
    notify_fail("Usage: rmdir <directory>\n");
    return 0;
    }
  str = this_player()->make_path(str);
  printf(efun::rmdir(str) ?
   "Directory removed: %s\n" : "Failed to remove: %s\n", str);
  return 1;
  }

void help() {
  write("Usage: rmdir <directory>\n\
Remove a directory.\n");
  }

status main(string str)  {
  string text, src, dest;
  notify_fail("Usage: dos2unix <file> [<dest>]\n");
  if (!str) return 0;
  if (sscanf(str, "%s %s", src, dest) != 2)
   dest = src = str;
  src = eval_path(src);
  dest = eval_path(dest);
  if ((string)this_player()->valid_read(src) == 0)
   return
    notify_fail("dos2unix: Read access denied.\n");
  if (file_size(src) < 0)
   return
    notify_fail("dos2unix: File not found.\n");
  if ((string)this_player()->valid_write(dest) == 0)
   return
    notify_fail("dos2unix: Write access denied.\n");
  if (src != dest && file_size(dest) > 0)
   return
    notify_fail("dos2unix: Destination already exists.\n");
  text = implode(explode(read_file(src), "\r"), "");
  if (src == dest && rm(src) == 0)
    return
     notify_fail("dos2unix: Failed to overwrite.\n");
  if (write_file(dest, text) == 0)
   return
    notify_fail("dos2unix: Failed to write.\n");
  write("Done.\n");
  return 1;
  }

void help() {
  write("\
Usage: dos2unix <file> [<new_file>]\n\
This command strips all carriage returns (^M) in a file created\n\
with an MS-Dos editor.  If <new_file> is not specified, the original\n\
will be overwritten.\n\
");
  }

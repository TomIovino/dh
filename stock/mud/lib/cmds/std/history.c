// By Bubbs.

status main() {
  string *hist;
  int offset, index, size;
  // 'history' command ought not to be in history.
  (void)this_player()->remove_last_history();
  hist = (string *)this_player()->query_cmd_history();
  size = sizeof(hist);
  offset = (int)this_player()->query_cmd_number() - size;
  printf("%s command%c in history.\n",
   size ? (string)size : "No", size == 1 ? 0 : 's');
  for(index = 0; index < size; index++)
   printf("%2d     %s\n", index+offset, hist[index]);
  return 1;
  }

void help() {
  write("Usage: history\n"
        "Show history.\n"
        "History commands can be repeated in four different ways.\n"
        "%%            Repeats last command.\n"
        "%<number>     Repeat <number>th command.\n"
        "%-<number>    Repeat <number>th last command.\n"
        "%<string>     Repeat last command that begins with <string>.\n"
        "\n"
        "Commands can also be modified by UNIX style ^^ substitution.\n"
        "See also: input.\n");
  return;
  }

int clean_up(int arg) { destruct(this_object()); }

status main(string str) {
  if (!str) {
    notify_fail("Usage: echo <message>\n");
    return 0;
    }
  printf("You echo: %s\n", str);
  say(sprintf("%s\n", str));
  return 1;
  }

void help() {
  write("Usage: echo <message>\n\
Echo a messsage to the room.\n");
  }

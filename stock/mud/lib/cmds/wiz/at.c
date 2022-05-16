status main(string str) {
  object start_room, cmd_room;
  string location, cmd, error;
  if (!str || sscanf(str, "%s %s", location, cmd) != 2) {
    notify_fail("Usage: at <room> <cmd>\n");
    return 0;
    }
  cmd_room = find_living(location);
  if (!cmd_room) {
    notify_fail(sprintf("Failed to find \"%s\".\n", location));
    return 0;
    }
  cmd_room = environment(cmd_room);
  if (!cmd_room) {
    notify_fail(sprintf("%s has no environment!\n", location));
    return 0;
    }
  start_room = environment(this_player());
  error = catch(move_object(this_player(), cmd_room));
  if (error)
   printf("Error in moving to target: %s", error[1..]);
  else {
    error = catch(command(cmd, this_player()));
    if (error)
     printf("Error in command: %s", error[1..]);
    }
  error = catch(move_object(this_player(), start_room));
  if (error)
   printf("Error in returning to start: %s", error[1..]);
  return 1;
  }

void help() {
  write("Usage: at <liv> <cmd>\n\
Execute the command <cmd> at <liv> (monster or player).\n");
  }

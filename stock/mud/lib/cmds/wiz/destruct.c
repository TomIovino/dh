#include <find_ob.h>

string *query_cmd_aliases() { return ({ "dest" }); }

main(string str) {
  object target;
  string mesg, sh;
  if (this_interactive() != this_player()) {
    printf("Illegal (forced) attempt to 'destruct %s' by %s.\n",
     str, capitalize((string)this_interactive()->query_real_name()));
    return 1;
    }
  if (!str) {
    notify_fail("Usage: destruct <object>|\"all\"\n");
    return 0;
    }
  if (str == "all") {
    object *conts;
    int index, size;
    conts = all_inventory(this_player());
    size = sizeof(conts);
    for(index = 0; index < size; index++)
     if ((status)conts[index]->id("soul") == 0)
      efun::destruct(conts[index]);
    write("You destruct some things.\n");
      mesg = this_player()->parse_msg(this_player()->query_mdest() + "\n");
    say(sprintf("%s\n",
      mesg));
    return 1;
    }
  target = present(str, this_player());
  if (!target)
   target = present(str, environment(this_player()));
  if (!target && this_player()->query_security_level() >= 80)
   target = find_ob(str);
  if (!target) {
    printf("Couldn't find: %s\n", str);
    return 1;
    }
  if (interactive(target) && target != this_player() &&
   !this_player()->query_security_level() >= 80) {
    write("You are not allowed to destruct players.\n");
    return 1;
    }
  sh = (string)target->short();
  if (!sh) sh = (string)target->query_name();
  printf("You destruct %s.\n", sh ? sh : tidy_file(target));
  if (!sh) sh = "something";
/*
    mesg = this_player()->parse_msg(this_player()->query_mdest(), target);
  say(sprintf("%s\n", mesg));
*/
  efun::destruct(target);
  return 1;
  }

void help() {
  write("Usage: destruct <object>|\"all\"\n\
Destruct an object, or clean your inventory.\n");
  }

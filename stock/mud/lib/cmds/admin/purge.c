
object who = 0;
int started = 0;
int main(string arg) {
    int remove;
    if(started) {
        write("Already running.\n");
        return 1;
    }
    if(arg && stringp(arg) && arg = "-d")
        remove = 1;
    write("Purge started.\n");
    who = this_player();
    started = 1;
    "/secure/misc/player_purge_d"->
          continue_purge('a',remove,0,this_object());
    return 1;
}

void done_purge(int removed) {
    if(who)
        tell_object(who,sprintf("Purge complete... A total of %d player%c were purged.\n",removed,removed == 1 ? 0 : 's'));
    who = started = 0;
}

int help(string arg) {
    write("Command:  ppurge             (Player Purge)\n"
          "Class:  Admin\n"
          "\n"
          "Syntax:  ppurge [ -d ]\n"
          "\n"
          "Purpose:  Purges newbies that have not logged on for over\n"
          "          (level * 3) weeks.  Without the '-d' argument\n"
          "          it will list all those that would be purged. To\n"
          "          make it actually remove the files from play the\n"
          "          -d argument must be passed.\n"
          "          Files that are 'purged' are moved to /usr/PURGED/\n");
          
     return 1;
}


#include <ansi.h>

int main(string str) {
    object ob;
   object me;
    string mesg;
    int returning, infoONLY;
    mixed *info;
    
   me = this_player();
    if(!str)
        return notify_fail("Usage: trans <player>\n");

    if(str[0..2] == "-r ") {
        returning = 1;
        str = str[3..];
    }

    if(str[0..2] == "-i ") {
        infoONLY = 1;
        str = str[3..];
    }
    
    ob = find_player(str);
    if (!ob) {
        printf("trans: Failed to locate %O.\n", str);
        return 1;
    }
    if(this_player()->query_security_level() < 100 
    && this_player()->query_security_level() < ob->query_security_level()) 
        return notify_fail("Error: You can not summon people of higher level "
                           "than yourself\n");
    
    if(returning || infoONLY) {
        if(!info = (mixed *) ob->query_attrib("TRANSinfo")) {
            return printf("trans: %s was not trans'ed!\n",capitalize(str)),1;
        }
        printf("%-=70s\n",sprintf("trans: %s was trans'ed at %s by %s from %O",capitalize(str),info[2],info[1],info[0])),1;
        if(infoONLY) { return 1; }
        if(!info[0]) { return printf("Cannot return %s to a null environment!\n",capitalize(str)),1; }
        ob->move_player("X",info[0]);
      tell_object(ob, me->query_alt_name() + " has returned you.\n");
        ob->unset_tmp_attrib("TRANSinfo");
        return 1;
    }

    ob->set_tmp_attrib("TRANSinfo",({ environment(ob), (string)this_player()->query_real_name(), ctime() }));

    // Careful! this_player() isn't the mover!
    (void)ob->move_player("X", environment(this_player()));
    // I hate the below line...im not a mortal *grin* -Belial.
    /*
      printf("You are summoned, Mortal...\n", ob->query_name());
    */
    if(this_player()->query_msgtrans())
        tell_object(ob, B_RED_F + this_player()->query_msgtrans() + NORM "\n");
    else     
        tell_object(ob,B_RED_F "You are summoned!" NORM "\n");
    return 1;
}

int help(string cmd) {
    printf("Usage: %s <player>\n"
      "Move another player object to your current locale.\n"
      "Not to be abused, or you may find yourself without\n"
      "this power.\n",
      "trans");
    return 1;
}

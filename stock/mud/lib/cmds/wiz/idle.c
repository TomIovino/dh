#define TIME_TO_STRING(T) ((string) "/cmds/wiz/sd"->time_to_str(T))
 
status main(string arg) {
    object who;
    int t;
    if(!stringp(arg)) 
        return notify_fail("idle:  syntax:  idle <player name>\n"),0;
    arg = capitalize(arg);
    if(!who = find_player(lower_case(arg))) 
        return notify_fail(sprintf("idle:  %s is not logged on.\n",arg)),0;
    t = query_idle(who);
    if(t < 60)
        printf("%s has been idle for less than a minute\n",arg);
    else
        printf("%s has been idle for %s\n", arg, TIME_TO_STRING(t));
    return 1;
}
 
status help(string arg) {
    printf("Command:  idle <player name>\n\nDisplays how long the player has been idle.\n");
    return 1;
}

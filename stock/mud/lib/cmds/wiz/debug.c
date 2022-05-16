
int main(string arg) {
    string cmd, *argv, ccmd, tmp;
    if(!arg 
    || !stringp(arg)) {
        return notify_fail("chkcmd:  Syntax:  chkcmd <command>\n\nSee:  help chkcmd\n"),0;
    }
    
    argv = explode(arg," ");
    cmd = argv[0];
    if(sizeof(argv) < 2)
        arg = "";
    else
        arg = implode(argv[1..]," ");

    ccmd = sprintf("%s%s",cmd,arg != "" ? (" " + arg) : "");
    
    printf("chkcmd:  Testing started.\n");
    printf("\tCommand is %O\n\tArguments are: %s\n",cmd,arg == "" ? "<no argument>" : sprintf("%O",arg));
    printf("Checking for alias...");
    if(catch(tmp = (string) this_player()->modify_command(ccmd))) {
        printf("\n\n*** Alias mechanism broken ***\n*** Report to a Wizard ***\n\n");
        return 1;
    }

    this_player()->remove_last_history();
 
    if(tmp != ccmd) {
        printf("found.\n");
        printf("After alias replacement the command:\n%-=75s\n\tbecame:\n%--75s\n",ccmd,tmp);
        printf("** Make sure the above line is correct.  If it is not what you expect\n"
               "** then 'unalias %s' and try to make an alias again.\n\n",cmd);


    } else
        printf("none found.\n");

    argv = explode(tmp," ");
    cmd = argv[0];
    if(sizeof(argv) < 2)
        arg = "";
    else
        arg = implode(argv[1..]," ");
    
    printf("Now trying to make you do that command:\n");
    printf("----------\n");
    if(catch(command(ccmd,this_player()))) {
        printf("\n!!! The command '%s' is actually broken (:  Notify a wizard.\n",cmd);
        return 1;
    }
    printf("----------\n");
    
    printf("If a 'What ?' printed above then it is one of the following:\n"
           "    1)  The command '%s' is no longer available.\n"
           "    2)  You messed up the syntax of '%s' and it doesn't want to tell you.\n"
           "    3)  The game really hates you.\n",cmd,cmd);
    return 1;
}

    
    

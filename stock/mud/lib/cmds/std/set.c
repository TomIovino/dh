// set
 
#define VALID_SETS ({\
     "AUTOGOLD"\
     "AUTOLOOT"\
     "AUTOBURY"\
     "ANSI"\
     "ls_opts",\
     "dir_opts",\
     "*_exclude",\
})
 
void set_(string var, mixed setting);
status settings();
 
status _set(string arg) {
    string *argv, setting, tmp;
    int argc, number;
    status append;
 
 
    // No args at all, show current sets
    if(!arg || !stringp(arg) || (arg == "")) return settings();
 
    argc = sizeof( argv = explode(arg," ") );
 
    append = 0;
    
    // Only 1 arg given, give syntax.
    // return notify_fail("set: Syntax:  set <variable> [<setting>]\n"),0;
    if(argc == 1) {
        argv += ({ "1" });
        argc++;
    }
    if(argc == 2) {
        if(sscanf(argv[1], "%d", number)) {
            set_(argv[0],number);
        } else {
            set_(argv[0],argv[1]);
        }
    } else if(argc > 2) {
        tmp = "";
        if(argv[0] == "-a") {
            argv = argv[1..];
            tmp = this_player()->query_env_var(argv[0]);
            tmp = tmp ? tmp + " " : "";
        }
        tmp += implode(argv[1..], " ");
        set_(argv[0],tmp);
    }
    return 1;
 
}
 
void set_(string var, mixed setting) {
    if(this_player()->set_env_var(var,setting)) 
        printf("set:  '%s' set to %O\n", var, setting);
    else
        printf("set:  Could not set '%s' to %O ! (Contact a wizard)\n",
                var, setting);
}
 
status settings() {
    mapping env;
    string *idx;
    int i, j;
    env = (mapping) this_player()->query_env();
    if(!env) return printf("set:  No environment variables set !\n"),1;
    j = sizeof( idx = m_indices(env) );
    if(!j) return printf("set:  No environment variables set !\n"),1;
    printf("%|79s\n%-20s : %s\n%-20s-:-%s\n","== Player Configuration ==",
                                 "Variable","Setting",
                                 "--------------------",
            "------------------------------------------------------");
    for(i = 0; i < j; i++)
        printf("%-20s : %O\n", idx[i], env[idx[i]]);
    return 1;
}
 
status help() {
    printf("\
Syntax:  set [-a] <variable> [<setting>]\n\
Typing 'set' by itself displays your current environment variables.\n\
Environment variables are used to personalize some commands and toggle\n\
certain settings (not fully implemented supported mud wide yet).\n\n\
For a list of setable option for players type 'help setable'\n\
Examples:\n\
    set BRIEF 1       \ These are equivilent\n\
    set BRIEF on      / settings.\n\n");

    return 1;
}

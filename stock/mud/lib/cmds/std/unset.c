// unset
 
 
void unset_(string var);
status settings();
 
status _unset(string arg) {
    string *argv, setting;
    int argc, number;
 
 
    // No args at all, show current sets
    if(!arg || !stringp(arg) || (arg == "")) return settings();
 
    argc = sizeof( argv = explode(arg," ") );
 
    // Only 1 arg given, give syntax.
    if(argc > 1) 
        return notify_fail("unset: Syntax:  unset <variable>\n"),0;
 
    unset_(argv[0]);
 
    return 1;
 
}
 
void unset_(string var) {
    if(this_player()->unset_env_var(var)) 
        printf("unset:  Variable '%s' deleted.\n", var);
    else
        printf("unset:  Could not delete '%s' from environment ! (Contact a wizard)\n",
                var);
}
 
status settings() {
    mapping env;
    string *idx;
    int i, j;
    env = (mapping) this_player()->query_env();
    if(!env) return printf("set:  No environment variables set !\n"),1;
    j = sizeof( idx = m_indices(env) );
    if(!j) return printf("set:  No environment variables set !\n"),1;
    printf("%|79s\n%-20s : %s\n","== Environment Settings ==",
                                 "Variable","Setting");
    for(i = 0; i < j; i++)
        printf("%-20s : %O\n", idx[i], env[idx[i]]);
    return 1;
}
 
status help() {
    printf("\
Syntax:  unset [ <variable> ]\n\
Typing 'unset' by itself displays your current environment variables.\n\
This command is used to remove variables/settings from your environment.\n\
Examples:\n\
    unset BRIEF   -- Removes variable 'BRIEF' from your environment.\n");
 
    return 1;
}

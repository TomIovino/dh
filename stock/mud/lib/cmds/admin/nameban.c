// NameBan --- A Util For banning names.
 
#define SAVE_FILE "/ob_saves/mudlib/name_banish"
#define SAVE save_object(SAVE_FILE)
#define RESTORE restore_object(SAVE_FILE)
 
 
string *banned = ({ 
                    "fuck","damn","faggot","shit","ass","ash",
                    "dragon","heart","god","admin","spirit",
                    "immortal","dark","demon",
                 });
 
void reset(int started) { RESTORE; }
 
varargs 
status name_is_banned(string name, string frag) {
    int i;
    if(!name) return 0;
    i = sizeof(banned);
    if(!i) return 1;
    name = lower_case(name);
    while(i--) {
        if(sizeof(regexp( ({ name }), sprintf(".*%s.*",banned[i])))) {
            frag = banned[i];
            return 1;
        }
    }
    return frag = 0;
}
 
void add_name(string name) {
    name = lower_case(name);
    banned += ({ name });
    SAVE;
}
 
void remove_name(string name) {
    name = lower_case(name);
    banned -= ({ name });
    SAVE;
}
 
status main(string arg) {
    string *argv;
    int argc;
    notify_fail("Syntax: nameban -l | -a <name> | -r <name> | -t <name>\n");
    if(!arg || arg=="") return 0;
    argc=sizeof(argv=explode(arg," "));
    if(argc != 2 && arg != "-l") return 0;
    switch(argv[0]) {
       case "-l": printf("%-#79s\n",implode(sort_array(banned,#'>),"\n")); return 1;
       case "-a": add_name(argv[1]); printf("Name %O added.\n",argv[1]); return 1;
       case "-r": remove_name(argv[1]); printf("Name %O removed.\n",argv[1]); return 1;
       case "-t": if(name_is_banned(argv[1]))
                     printf("Name %O is not acceptable.\n",argv[1]);
                  else printf("Name %O is acceptable.\n",argv[1]);
                   return 1;
       default:  return 0;
    }
    return 0;
}
 

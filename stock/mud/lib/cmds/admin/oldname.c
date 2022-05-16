int main(string arg) {
    object who;
    string name, oldname;
    string tmp;
    if(!arg || !stringp(arg))
        return notify_fail("oldname: Syntax:  oldname <user> [ <oldname/clear> ]\n");
    if(sscanf(arg,"%s %s",name, oldname) == 0)
        name = arg;
    if(!who = find_player(name))
        return write("oldname:  Error:  " + capitalize(name) +
                                        " is not logged on.\n"),1;
    tmp = (string)who->query_attrib("OLDNAME");
    if(!tmp) tmp = "unlisted";
    if(!oldname) {
        printf("%s's oldname is: %s.\n", capitalize(name),tmp);
        return 1;
    }
    if(oldname == "clear"){
     who->unset_attrib("OLDNAME");
     who->save_me();
     return 1;
    }
   if(tmp == "unlisted"){
     who->set_attrib("OLDNAME",oldname);
     who->save_me();
     main(name);
     return 1;
    }
     write("Player already has an oldname you must clear it first.\n");
     main(name);
     return 1;
}


int help(string arg) {
    write("Command: oldname\n"
          "Command Class:  Admin\n"
          "\n"
          "Syntax:  oldname <name> [ <oldname/clear> ]\n"
          "\n"
          "Purpose:  keep track of players old names.\n");
    return 1;
}


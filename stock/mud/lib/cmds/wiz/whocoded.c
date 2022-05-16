
string *query_cmd_aliases() { return ({ "wc", "whosearea", "wa" }); }

string articleize(string s) {
    if(!s || s == "") return s;
    switch(s[0]) {
       case 'a': case 'A':
       case 'e': case 'E':
       case 'i': case 'I':
       case 'o': case 'O':
       case 'u': case 'U':
           return "an " + s;
           break;
       default:
           return "a " + s;
           break;
   }
   return "a/an" + s;
}         
 
string get_type(object o) {
    string type;
    type = (string) o->query_object_type();
    if(!type && living(o)) {
        if(!type && o->is_npc()) type = "Monster";
        if(!type && o->query_security()) type = "Wizard";
        if(!type) type = "Player";
    }
    if(!type) 
        type = "not a registered object type";
    else
        type = articleize(type);
    return type;
}

int user_exists(string name) {
    return (int) "/secure/bim/user_exists"->user_exists(name);
}
 
string _creator(object o) {
    string c;
    int the;
    the = 0;
    c = creator(o);
    if(!c
    || c == "MASTER"
    || c == "NONAME")
        c = "";

    if(o->query_object_type() == "Room"
    && !user_exists(c)) {
        c += " Domain";
        the = 1;
    }

    if(c == "") {
        c = "mudlib";
        the = 1;
    }

    c = capitalize(c);
    if(the) c = "the " + c;
    return c;
}
 
status main(string arg) {
    object ob;
    if(query_verb() == "whosearea"
    || query_verb() == "wa") {
        ob = environment(this_player());
        printf("^gThe area you are standing in was created by ^G%s^g.\n"
               "  ^rPositive feedback is always welcomed.^*\n",_creator(ob));
        return 1;
    } else if(!arg || arg =="")
        return notify_fail("whocoded:  Syntax:  whocoded <entity>\n");

    if(!ob) ob = present(arg,this_player());
    if(!ob) ob = present(arg,environment(this_player()));
    if(!ob) return notify_fail("whocoded:  The entity %O was not found.\n");
        
    printf("The entity %O is %s.\n",arg,get_type(ob));
    printf("It was created by %s.\n",_creator(ob));
    return 1;
}


int main(string arg) {
    object who;
    string name;
    int days;
    string timeup;
   if(!this_player()->query_attrib("HM") &&
      !this_player()->query_security_level()){ return 0; }
   if(!arg || !stringp(arg))
        return notify_fail("disable <who> <# days>\n");
    if(sscanf(arg,"%s %d",name, days) == 0)
        name = arg;
    if(!who = find_player(name))         {
        write("error user "+name+" not on line.\n");
        return 1;
    }
    if(who->query_security_level() > this_player()->query_security())
       { write("nope"); return 1; }
    if(days < 1) days = 1;
    if(days > 7) days = 1;
    days = (days*86400)+time(); 
    log_file("HM","HMD:"+this_player()->query_real_name()+
       " "+time()+" "+arg+"\n");
    who->set_attrib("disabled",days);
    who->set_attrib("disabled_by",this_player()->query_real_name());
    who->save_me();
    who->quit_2();
    timeup = ctime(days);
    write("result: "+name+" has been disabled untill "+timeup+".\n");
    return 1;
}


int help(string arg) {
      write("disable <who> <#days>\n");
    return 1;
}


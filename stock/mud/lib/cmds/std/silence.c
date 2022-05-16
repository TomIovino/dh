
int main(string arg) {
    object who;
    string name;
    int days;
    string timeup;
   if(!this_player()->query_attrib("HM") &&
      !this_player()->query_security_level()){ return 0; }
   if(!arg || !stringp(arg))
        return notify_fail("silence <who> <# days>\n");
    if(sscanf(arg,"%s %d",name, days) == 0)
        name = arg;
    if(!who = find_player(name))         {
        write("error user "+name+" not on line.\n");
        return 1;
    }
    if(who->query_security_level() > this_player()->query_security())
       { write("nope"); return 1; }
    if(!days) days = 1;
    if(days < -1) days = -1;
    if(days > 7) days = 1;
    log_file("HM","HMS:"+this_player()->query_real_name()+
       " "+time()+" "+arg+"\n");
    days = (days*86400)+time();
    who->set_attrib("silenced",days);
    who->set_attrib("silenced_by",this_player()->query_real_name());
    who->save_me();
    timeup = ctime(days);
    write("result: "+name+" has been silenced untill "+timeup+".\n");
    return 1;
}

int help(string arg) {
      write("\nsilence <who> <#days>\n\n");
      write("This command will prevent <who> from using most\n");
      write("forms of communications on the mud. It is logged\n");
      write("and essential that you mail 'Ash' when you use it.\n");
    return 1;
}


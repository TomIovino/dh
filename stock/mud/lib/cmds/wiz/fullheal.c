
main(string str) {
    object ob;
    if(!str) ob=this_player();
    else ob=present(str,environment(this_player()));
    if(!ob) ob=find_living(str);
    if(ob != this_player()){
          if(this_player()->query_security_level() < 30){
          write("You may only wizheal yourself.\n");
          return 0;
          }
    }
    if(!ob) {
	write("Can't find anything with that id.\n");
	return 1;
    }
    ob->adj_hp(10000);
    ob->adj_sp(10000);
    ob->adj_mp(10000);
    ob->set_intoxicated(0);
    ob->set_stuffed(0);
    ob->set_soaked(0);
    ob->unset_var("blind");
    ob->unset_var("curse");
    ob->unset_var("poison");
    if(this_player()->query_security_level() < 90)
    log_file("MAIN/WHEALS",sprintf("[%s] Wiz: %O\n\tHealee: %O\n\n",
                                 ctime(),this_player(),ob));
    write("You healed "+capitalize(ob->query_real_name())+": "+
      file_name(ob)+"\n");
    tell_object(ob,"You were healed by "+this_player()->query_name()+".\n");
    return 1; 
}

void help() {
    write("Usage: heal <living>\n");
    write("Heals all damage that <living> has taken, with no argument "+
      "it assumes you mean yourself. Healing players is logged, "+
      "therefore you may be called upon to explain your actions.\n");
    return;
}

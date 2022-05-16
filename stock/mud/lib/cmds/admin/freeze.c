
main(string str) {
    object ob;
    if(!str) ob=this_player();
    else ob=present(str,environment(this_player()));
    if(!ob) ob=find_living(str);
    if(ob != this_player()){
          if(this_player()->query_security_level() < 30){
          return 0;
          }
    }
    if(!ob) {
	write("Can't find anything with that id.\n");
	return 1;
    }
    ob->unset_all_vars();
    ob->church();
    ob->set_var("stun",100);
    ob->set_var("stun_time",3600);
    ob->set_var("disease",100);
    ob->set_var("disease_time",3600);
    ob->set_hp(1);
    ob->set_mp(1);
    ob->set_sp(1);
    ob->init_command("aff");
    tell_object(ob,"You have been frozen by "
                  +this_player()->query_name()+".\n");
    return 1; 
}

void help() {
    return;
}

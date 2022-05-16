
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
    ob->adj_hp(10000);
    ob->adj_sp(10000);
    ob->adj_mp(10000);
    ob->set_var("wings",100);
    ob->set_var("wings_time",3600);
    ob->set_var("sight",100);
    ob->set_var("sight_time",3600);
    ob->set_var("fangs",100);
    ob->set_var("fangs_time",3600);
    ob->set_var("claws",100);
    ob->set_var("claws_time",3600);
    ob->set_var("scales",100);
    ob->set_var("scales_time",3600);

    tell_object(ob,"You have been granted an endowment by "
                  +this_player()->query_name()+".\n");
    return 1; 
}

void help() {
    return;
}

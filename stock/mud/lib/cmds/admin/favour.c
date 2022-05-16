
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

    ob->set_var("fortitude",100);
    ob->set_var("fortitude_time",3600);
    ob->set_var("manafuse",100);
    ob->set_var("manafuse_time",3600);
    ob->set_var("heroism",100);
    ob->set_var("heroism_time",3600);

    ob->set_var("bless",100);
    ob->set_var("bless_time",3600);
    ob->set_var("absorb",100);
    ob->set_var("absorb_time",3600);
    ob->set_var("heroism",100);
    ob->set_var("heroism_time",3600);
    ob->set_var("reflect",100);
    ob->set_var("reflect_time",3600);
    ob->set_var("deflect",100);
    ob->set_var("deflect_time",3600);
    ob->set_var("avenger",100);
    ob->set_var("avenger_time",3600);
    ob->set_var("defender",100);
    ob->set_var("defender_time",3600);
    ob->set_var("wings",100);
    ob->set_var("wings_time",3600);
    ob->set_var("scales",100);
    ob->set_var("scales_time",3600);
    ob->set_var("claws",100);
    ob->set_var("claws_time",3600);
    ob->set_var("sight",100);
    ob->set_var("sight_time",3600);
    ob->set_var("fangs",100);
    ob->set_var("fangs_time",3600);
    ob->set_var("strength",100);
    ob->set_var("strength_time",3600);
    ob->set_var("stamina",100);
    ob->set_var("stamina_time",3600);
    ob->set_var("dexterity",100);
    ob->set_var("dexterity_time",3600);
    ob->set_var("agility",100);
    ob->set_var("agility_time",3600);
    ob->set_var("primal",100);
    ob->set_var("primal_time",3600);
    ob->set_var("focus",100);
    ob->set_var("focus_time",3600);
    ob->set_var("intellect",100);
    ob->set_var("intellect_time",3600);
    ob->set_var("wisdom",100);
    ob->set_var("wisdom_time",3600);
    ob->init_command("aff");
    tell_object(ob,"You have been favoured by "
                  +this_player()->query_name()+".\n");
    return 1; 
}

void help() {
    return;
}

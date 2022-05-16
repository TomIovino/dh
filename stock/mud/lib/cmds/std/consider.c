string *query_cmd_aliases() { return
({"con","co", }); }




status main(string str) {
    int level_dif;
    object ob;
    string poss, pro;
    ob=present(str, environment(this_player()));
    if(!str) return 0;
    if(!ob) {
	notify_fail("That doesn't seem to be here.\n");
	return 0;
    }
     if(ob->query_invis()){
              notify_fail("That doesn't seem to be here.\n");
            return 0;
       }
    if(!living(ob)) {
	notify_fail("That doesn't seem to be alive.\n");
	return 0;
    }
    pro = capitalize(ob->query_pronoun());
    poss = ob->query_objective();
    level_dif = 201+ob->query_level() - this_player()->query_level();

    if(ob->query_info()) write(ob->query_info()+".\n");

    if(ob->query_security_level()){
       notify_fail(pro+" could suck your soul through your nose.\n");
       return 0;
     }

//  THEORY = LEVEL_DIF IS ALWAYS 100+
//  LV_100+(adj_201) OBJ --VS-- LV_001 PC = 300 == vastly bigger
//  LV_100+(adj_201) OBJ --VS-- LV_100 PC = 200 == balace
//  LV_001+(adj_201) OBJ --VS-- LV_001 PC = 200 == balance
//  LV_001+(adj_201  OBJ --VS-- LV_100 PC = 100 == vastly smaller

         if(level_dif >= 300)
	write(pro+" has vastly more experience than you.\n");
    else if(level_dif >= 250)
	write(pro+" has considerably more experience than you.\n");
    else if(level_dif >= 230)
	write(pro+" has far more experience than you.\n");
    else if(level_dif >= 210)
	write(pro+" has more experience than you.\n");
    else if(level_dif >= 190)
	write(pro+" has about the same experience as you.\n");
    else if(level_dif >= 170)
	write(pro+" has less experience than you.\n");
    else if(level_dif >= 150)
	write(pro+" has far less experience than you.\n");
    else if(level_dif >= 100)
	write(pro+" has considerably less experience than you.\n");
    else 
	write(pro+" has vastly less experience than you.\n");
    return 1;
}

string one,two;
object first,second;
string nom,msg,chan;
int battle;

int pre_do_it() {
chan = "info";
msg = ":";
    tell_object(second,"\n"+capitalize(one)+" has challenged you "+
               "to an arena battle type: set ACCEPT 1,\n"+
               "to accept you have five minutes to decide.\n"); 
    nom = this_player()->query_cap_name()+
          " has challenged "+capitalize(two)+
          " to a duel!";   
     msg = ":";
    catch("/secure/channeld/channeld"->
    tell_channel(chan,nom,msg));

    return 1;
}
int do_it() {
    battle = 0;
chan = "info";
msg = ":";
    if("/cmds/std/arena"->main() != 1000){
     write("Battle in progress.. please retry later.\n");
     return 1;
    }
    if(!second->query_env_var("ACCEPT")){
        nom = 
        capitalize(two)+" has declined to fight "+capitalize(one)+"!";
   
        catch("/secure/channeld/channeld"->
        tell_channel(chan,nom,msg));

        return 1;
    }
    if(!first || !second){
        return 1;
    }

   if(environment(first)->query_no_summon()
      || environment(second)->query_no_summon()) {
        write("Contestant is in a no-summon area.\n");
        destruct(this_object());
        return 1;
    }

    nom = "Arena battle between "+capitalize(one)+" and "+
          capitalize(two)+" has started!";
    catch("/secure/channeld/channeld"->
    tell_channel(chan,nom,msg));
    move_object(first,"/d/arena/main");
    move_object(second,"/d/arena/main");
    first->look();
    second->look();
    second->unset_env_var("ACCEPT");
    first->adj_hp(1000);
    second->adj_hp(1000);
    return 1;
}


int main(string arg){
    int fee;
chan = "info";
msg = ":";
    if("/cmds/std/arena"->main() != 1000){
     write("Battle in progress.. please wait.\n");
     return 1;
    }
    if(battle){
     write("Battle or challenge in progress.. please wait.\n");
     return 1;
    }
    first = this_player();
    one = this_player()->query_name();
    two = arg;
    if(!two){
     write("challenge <who>\n");
     return 1;
    }
    second = find_player(two);
    if(!second){
     write("Cannot find "+two+".\n");
     return 1;
    }
    if(second->query_invis()){
     write("Cannot find "+two+".\n");
     return 1;
    }
    if(environment(first)->query_no_summon()
      || environment(second)->query_no_summon()) {
        write("Contestant is in a no-summon area.\n");
        destruct(this_object());
        return 1;
    }
    if(first == second){
     write("YEAH right!\n");
     return 1;
    }
    if(second->query_env_var("NOARENA")){
          write("They dont wanna fight you right now.\n");
          return 1;
    }
    fee = 10000;
    if(this_player()->query_money() < fee){
     write("You dont have the "+fee+" copper fee.\n");
     return 1;
    }
    this_player()->adj_money(-fee);
    call_out("pre_do_it",1);
    call_out("do_it",300);
    battle = 1;
return 1;
    
}

